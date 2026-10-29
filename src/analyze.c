#define _DEFAULT_SOURCE
#include "ibp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
static int check_tool(const char *tool){
    const char *env=getenv("PATH"); char *copy,*save,*dir; if(!env)return 0;
    copy=strdup(env); if(!copy)return 0;
    for(dir=strtok_r(copy,":",&save);dir;dir=strtok_r(NULL,":",&save)){char p[1024];snprintf(p,sizeof(p),"%s/%s",*dir?dir:".",tool);if(access(p,X_OK)==0){free(copy);return 1;}}
    free(copy);return 0;
}
int cmd_analyze(const char *path){
    int fail=0,warn=0; const char *dev=path?path:default_serial_device();
    ui_heading("Analyze — environment readiness");
#define TOOL(t) do{char msg[160];snprintf(msg,sizeof(msg),"Required tool available: %s",t);if(check_tool(t))ui_ok(msg);else{snprintf(msg,sizeof(msg),"Required tool missing: %s",t);ui_fail(msg);fail++;}}while(0)
    TOOL("cc"); TOOL("cmake");
    if(access("/dev",R_OK|X_OK)==0)ui_ok("Device directory /dev is accessible");else{ui_fail("Cannot access /dev");fail++;}
    if(dev){char msg[600];snprintf(msg,sizeof(msg),"Serial transport candidate: %s",dev);if(access(dev,R_OK|W_OK)==0)ui_ok(msg);else{snprintf(msg,sizeof(msg),"Serial transport is not accessible: %s",dev);ui_warn(msg);warn++;}}else{ui_warn("No serial device detected; hardware checks cannot run");warn++;}
    const char *sdk=getenv("PICO_SDK_PATH");
    if(sdk && *sdk && access(sdk,R_OK|X_OK)==0)ui_ok("PICO_SDK_PATH points to an accessible SDK directory");else{ui_warn("PICO_SDK_PATH is unset or inaccessible; Pico firmware build is unavailable");warn++;}
    if(dev && access(dev,R_OK|W_OK)==0){
        char resp[IBP_LINE_MAX],kind[64],payload[384];double ms; int r=serial_request(dev,"IBP/1 HELLO\n",resp,sizeof(resp),1000,&ms);
        if(r==0 && protocol_parse_line(resp,kind,sizeof(kind),payload,sizeof(payload))==0 && strcmp(kind,"OK")==0)ui_ok("iBreakPoint protocol handshake succeeded");else{ui_warn("Protocol handshake not verified (device may run different firmware or be busy)");warn++;}
    }else{ui_info("Protocol compatibility not checked because no accessible device is available");warn++;}
    if(fail) puts("\nResult: NOT READY"); else if(warn) puts("\nResult: READY WITH WARNINGS"); else puts("\nResult: READY");
    return fail?2:(warn?1:0);
}
