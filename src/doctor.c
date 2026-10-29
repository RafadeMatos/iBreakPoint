#include "ibp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
static int ask(const char *path,const char *cmd,char *kind,size_t kn,char *payload,size_t pn,double *lat){
    char req[128],resp[IBP_LINE_MAX];
    snprintf(req,sizeof(req),"IBP/1 %s\n",cmd);
    int r=serial_request(path,req,resp,sizeof(resp),1200,lat);
    if(r!=0) return r;
    if(protocol_parse_line(resp,kind,kn,payload,pn)!=0) return -6;
    return 0;
}
int cmd_doctor(const char *action,const char *path){
    const char *dev=path?path:default_serial_device(); char k[64],p[384]; double lat=0; int rc;
    ui_heading("DocTor Who");
    if(!dev){ui_fail("No serial device detected. Connect the iBreakPoint RP2040 firmware.");return 2;}
    printf("Observed transport: %s\n",dev);
    if(access(dev,R_OK|W_OK)!=0){ui_fail("Transport is not accessible. Check user/group permissions (often dialout) and device path.");return 2;}
    ui_ok("Transport path is accessible");
    rc=ask(dev,"HELLO",k,sizeof(k),p,sizeof(p),&lat);
    if(rc!=0){if(rc==-5)ui_fail("Handshake timed out: no response received.");else if(rc==-6)ui_fail("Malformed or unsupported protocol response.");else ui_fail("Serial communication failed.");puts("Firmware identity: unavailable\nProtocol compatibility: not established");return 2;}
    if(strcmp(k,"OK")!=0){printf("Handshake response: %s %s\n",k,p);ui_fail("Firmware rejected handshake.");return 2;}
    ui_ok("Handshake accepted"); printf("Observed latency: %.2f ms\n",lat); printf("Firmware-declared fields: %s\n",p);
    if(strcmp(action,"check")==0){ui_ok("Protocol endpoint responded with a valid IBP/1 frame");return 0;}
    rc=ask(dev,"INFO",k,sizeof(k),p,sizeof(p),&lat);
    if(rc!=0 || strcmp(k,"OK")!=0){ui_warn("Firmware metadata unavailable; handshake succeeded but INFO did not return valid metadata.");return 1;}
    printf("Firmware metadata (declared): %s\n",p);
    rc=ask(dev,"CAPS",k,sizeof(k),p,sizeof(p),&lat);
    if(rc==0 && strcmp(k,"OK")==0) printf("Declared capabilities: %s\n",p); else ui_warn("Capabilities unavailable.");
    ui_info("This is a diagnostic and compatibility check, not a security assessment."); return 0;
}
