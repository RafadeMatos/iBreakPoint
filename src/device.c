#define _DEFAULT_SOURCE
#include "ibp.h"
#include <glob.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
static void list_pattern(const char *pattern, int *count){
    glob_t g; size_t i;
    if(glob(pattern,0,NULL,&g)==0){for(i=0;i<g.gl_pathc;i++){struct stat st; printf("  %s",g.gl_pathv[i]); if(stat(g.gl_pathv[i],&st)==0 && access(g.gl_pathv[i],R_OK|W_OK)==0) puts("  [read/write]"); else puts("  [permission restricted]"); (*count)++;}}
    globfree(&g);
}
int cmd_device_list(void){
    int count=0; ui_heading("Device inventory"); puts("Serial transports exposed by the host:");
    list_pattern("/dev/serial/by-id/*",&count); list_pattern("/dev/ttyACM*",&count); list_pattern("/dev/ttyUSB*",&count);
    if(!count){ui_warn("No serial devices found. Connect the Pico and check USB cable/permissions.");return 1;}
    ui_info("USB identifiers are not firmware identity; a protocol handshake is required."); return 0;
}
int cmd_device_info(const char *path){
    const char *p=path?path:default_serial_device();
    ui_heading("Device information");
    if(!p){ui_warn("No serial transport detected.");return 1;}
    printf("Transport path: %s\n",p);
    if(access(p,R_OK|W_OK)==0) ui_ok("Transport exists and is readable/writable by this user.");
    else ui_warn("Transport exists or was selected, but access is restricted or unavailable.");
    ui_info("Hardware identity and firmware compatibility remain unverified until DocTor Who receives a valid response."); return 0;
}
