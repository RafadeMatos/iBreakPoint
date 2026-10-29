#define _DEFAULT_SOURCE
#include "ibp.h"
#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>

static double now_ms(void){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return (double)t.tv_sec*1000.0+(double)t.tv_nsec/1e6; }
const char *default_serial_device(void){
    static char found[512]; glob_t g;
    const char *env=getenv("IBP_DEVICE");
    if(env && *env) return env;
    if(glob("/dev/serial/by-id/*",0,NULL,&g)==0 && g.gl_pathc){ snprintf(found,sizeof(found),"%s",g.gl_pathv[0]); globfree(&g); return found; }
    globfree(&g);
    if(glob("/dev/ttyACM*",0,NULL,&g)==0 && g.gl_pathc){ snprintf(found,sizeof(found),"%s",g.gl_pathv[0]); globfree(&g); return found; }
    globfree(&g);
    if(glob("/dev/ttyUSB*",0,NULL,&g)==0 && g.gl_pathc){ snprintf(found,sizeof(found),"%s",g.gl_pathv[0]); globfree(&g); return found; }
    globfree(&g); return NULL;
}
int serial_request(const char *path,const char *request,char *response,size_t response_n,int timeout_ms,double *latency_ms){
    int fd; struct termios tio; size_t used=0; double start,deadline;
    if(!path || !request || !response || response_n<2) return -1;
    fd=open(path,O_RDWR|O_NOCTTY|O_NONBLOCK);
    if(fd<0) return -2;
    if(tcgetattr(fd,&tio)<0){close(fd);return -3;}
    cfmakeraw(&tio); cfsetispeed(&tio,B115200); cfsetospeed(&tio,B115200);
    tio.c_cflag|=(CLOCAL|CREAD); tio.c_cflag&=~CSTOPB; tio.c_cflag&=~PARENB; tio.c_cflag&=~CSIZE; tio.c_cflag|=CS8;
    if(tcsetattr(fd,TCSANOW,&tio)<0){close(fd);return -3;}
    tcflush(fd,TCIOFLUSH);
    if(write(fd,request,strlen(request))<0){close(fd);return -4;}
    start=now_ms(); deadline=start+(double)timeout_ms;
    while(used<response_n-1 && now_ms()<deadline){
        fd_set r; struct timeval tv; double left=deadline-now_ms(); int s;
        FD_ZERO(&r);FD_SET(fd,&r); tv.tv_sec=(long)(left/1000.0);tv.tv_usec=(long)((long)left%1000)*1000;
        s=select(fd+1,&r,NULL,NULL,&tv);
        if(s<0){if(errno==EINTR)continue;close(fd);return -4;}
        if(s==0) break;
        ssize_t n=read(fd,response+used,response_n-1-used);
        if(n>0){used+=(size_t)n;if(memchr(response,'\n',used))break;}
        else if(n<0 && errno!=EAGAIN && errno!=EINTR){close(fd);return -4;}
    }
    response[used]='\0'; if(latency_ms)*latency_ms=now_ms()-start; close(fd);
    return used?0:-5;
}
