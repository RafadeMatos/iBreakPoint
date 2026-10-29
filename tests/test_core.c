#include "ibp.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){
    char k[32],p[128];
    assert(protocol_parse_line("IBP/1 OK fw=0.1.0 proto=1\r\n",k,sizeof(k),p,sizeof(p))==0);
    assert(strcmp(k,"OK")==0);assert(strcmp(p,"fw=0.1.0 proto=1")==0);
    assert(protocol_parse_line("IBP/2 OK x\n",k,sizeof(k),p,sizeof(p))<0);
    assert(protocol_parse_line("nonsense\n",k,sizeof(k),p,sizeof(p))<0);
    assert(protocol_parse_line("IBP/1 \n",k,sizeof(k),p,sizeof(p))<0);
    char longline[256];memset(longline,'x',sizeof(longline));longline[sizeof(longline)-1]=0;
    assert(protocol_parse_line(longline,k,sizeof(k),p,sizeof(p))<0);
    ui_init("never");assert(ibp_color_enabled==0);
    puts("All core tests passed.");return 0;
}
