#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#define LINE_MAX_IBP 160
static void reply(const char *kind,const char *payload){printf("IBP/1 %s %s\n",kind,payload?payload:"");fflush(stdout);}
int main(void){
    stdio_init_all(); char line[LINE_MAX_IBP]; size_t n=0; absolute_time_t last=get_absolute_time();
    while(true){
        int c=getchar_timeout_us(10000);
        if(c==PICO_ERROR_TIMEOUT){if(n && absolute_time_diff_us(last,get_absolute_time())>500000)n=0;continue;}
        last=get_absolute_time();
        if(c=='\r')continue;
        if(c=='\n'){
            line[n]='\0';
            if(strcmp(line,"IBP/1 HELLO")==0)reply("OK","device=RP2040 protocol=1");
            else if(strcmp(line,"IBP/1 INFO")==0)reply("OK","firmware=0.1.0 build=dev target=RP2040");
            else if(strcmp(line,"IBP/1 CAPS")==0)reply("OK","hello,info,caps");
            else reply("ERR","code=BAD_COMMAND");
            n=0;continue;
        }
        if(n<LINE_MAX_IBP-1)n++;else{n=0;reply("ERR","code=LINE_TOO_LONG");}
        if(n>0)line[n-1]=(char)c;
    }
}
