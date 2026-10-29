#define _POSIX_C_SOURCE 200809L
#include "ibp.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <stdlib.h>
#include <ctype.h>
static volatile sig_atomic_t interrupted=0;
static void on_int(int sig){(void)sig;interrupted=1;}
void print_help(void){
    puts("iBreakPoint — Embedded Firmware Robustness Testing\n\nUsage:\n  ibreakpoint [--help] [--version] [--color=auto|always|never]\n  ibreakpoint about\n  ibreakpoint device list\n  ibreakpoint device info [DEVICE]\n  ibreakpoint doctor inspect [DEVICE]\n  ibreakpoint doctor check [DEVICE]\n  ibreakpoint analyze [DEVICE]\n\nDEVICE defaults to IBP_DEVICE, then /dev/serial/by-id, /dev/ttyACM*, /dev/ttyUSB*.\nThe host uses 115200 baud. Hardware commands require the iBreakPoint protocol firmware.");
}
static int eq(const char*a,const char*b){return a&&b&&strcasecmp(a,b)==0;}
static int dispatch(int argc,char **argv){
    if(argc==0)return -1;
    if(eq(argv[0],"help")||eq(argv[0],"h")){print_help();return 0;}
    if(eq(argv[0],"about")||eq(argv[0],"a")){ui_about();return 0;}
    if(eq(argv[0],"device")||eq(argv[0],"1")){
        if(argc<2||eq(argv[1],"list"))return cmd_device_list();
        if(eq(argv[1],"info"))return cmd_device_info(argc>2?argv[2]:NULL);
        ui_fail("Unknown Device command. Use: device list | device info [DEVICE]");return 2;
    }
    if(eq(argv[0],"doctor")||eq(argv[0],"2")){
        const char *act=argc>1?argv[1]:"inspect";if(!eq(act,"inspect")&&!eq(act,"check")){ui_fail("Use: doctor inspect | doctor check [DEVICE]");return 2;}
        return cmd_doctor(act,argc>2?argv[2]:NULL);
    }
    if(eq(argv[0],"analyze")||eq(argv[0],"3"))return cmd_analyze(argc>1?argv[1]:NULL);
    ui_fail("Unknown command. Type help for available commands.");return 2;
}
static void interactive(void){
    char line[256]; bool easter=0; signal(SIGINT,on_int);
    for(;;){
        if(interrupted){puts("\nInterrupted. Returning to menu.");interrupted=0;}
        ui_banner(easter);ui_menu();printf("\nibp> ");fflush(stdout);
        if(!fgets(line,sizeof(line),stdin)){puts("");break;}
        line[strcspn(line,"\r\n")]=0;
        if(!*line)continue;
        if(eq(line,"q")||eq(line,"quit"))break;
        if(eq(line,"a")||eq(line,"about")){
            ui_about();printf("about> ");fflush(stdout);
            if(!fgets(line,sizeof(line),stdin)) break;
            line[strcspn(line,"\r\n")]=0;
            if(strcmp(line,"14012021")==0){easter=1;ui_banner(1);}else if(*line)ui_info("No hidden entry matched.");
            continue;
        }
        char *args[8];int n=0;char *save=NULL;for(char *t=strtok_r(line," \t",&save);t&&n<8;t=strtok_r(NULL," \t",&save))args[n++]=t;
        if(n)dispatch(n,args);
        puts("");
    }
}
int cli_main(int argc,char **argv){
    const char *color="auto"; int i=1;
    for(;i<argc;i++){
        if(strncmp(argv[i],"--color=",8)==0){color=argv[i]+8;if(strcmp(color,"auto")&&strcmp(color,"always")&&strcmp(color,"never")){fprintf(stderr,"Invalid color mode. Use auto, always, or never.\n");return 2;}}
        else break;
    }
    ui_init(color);
    if(i==argc){interactive();return 0;}
    if(strcmp(argv[i],"--help")==0||strcmp(argv[i],"-h")==0){print_help();return 0;}
    if(strcmp(argv[i],"--version")==0){puts("iBreakPoint " IBP_VERSION);return 0;}
    return dispatch(argc-i,argv+i);
}
