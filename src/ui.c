#define _POSIX_C_SOURCE 200809L
#include "ibp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int ibp_color_enabled = 0;
static const char *C_MAG="\033[35m", *C_GRN="\033[32m", *C_GREY="\033[90m", *C_YEL="\033[33m", *C_RED="\033[31m", *C_RST="\033[0m";
void ui_init(const char *mode) {
    const char *nc=getenv("NO_COLOR");
    if (mode && strcmp(mode,"always")==0) ibp_color_enabled=1;
    else if (mode && strcmp(mode,"never")==0) ibp_color_enabled=0;
    else ibp_color_enabled=isatty(STDOUT_FILENO) && (!nc || !*nc);
}
static const char *cc(const char *c) { return ibp_color_enabled ? c : ""; }
void ui_info(const char *s){ printf("%s[i]%s %s\n",cc(C_GREY),cc(C_RST),s); }
void ui_ok(const char *s){ printf("%s[PASS]%s %s\n",cc(C_GRN),cc(C_RST),s); }
void ui_warn(const char *s){ printf("%s[WARN]%s %s\n",cc(C_YEL),cc(C_RST),s); }
void ui_fail(const char *s){ printf("%s[FAIL]%s %s\n",cc(C_RED),cc(C_RST),s); }
void ui_heading(const char *s){ printf("\n%s== %s ==%s\n",cc(C_MAG),s,cc(C_RST)); }
static const char *berry =
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⠓⠶⣤⠀⠀⠀⠀⣠⠶⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠇⠀⢠⡏⠀⠀⢀⡔⠉⠀⢈⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠩⠤⣄⣼⠁⠀⣠⠟⠀⠀⣠⠏⠀⠀⢀⣀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⢀⣀⣀⣀⣀⣀⣀⣀⠀⠀⠀⠁⠀⠀⠣⣤⣀⡼⠃⠀⢀⡴⠋⠈⠳⡄⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣶⣿⡿⠿⠿⠟⠛⠛⠛⠛⠿⠿⣿⣿⣶⣤⣄⠀⠀⠀⠉⠀⢀⡴⠋⠀⠀⣠⠞⠁⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣾⣿⠿⠋⠉⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠻⢿⣿⣶⣄⠀⠀⠳⣄⠀⣠⠞⢁⡠⢶⡄⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⠿⠋⠀⠀⢀⣴⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠑⢤⡈⠛⢿⣿⣦⡀⠈⠛⢡⠚⠃⠀⠀⢹⡆⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⠟⠁⠀⠀⠀⢀⣾⠃⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⢻⡆⠀⠀⢻⣦⠀⠙⢿⣿⣦⡀⠈⢶⣀⡴⠞⠋⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⣠⣿⡿⠃⠀⠀⠀⠀⢀⣾⡇⢀⡄⠀⢸⡇⠀⠀⠀⠀⠀⠀⣀⠀⢸⣷⡀⠀⠀⠹⣷⡀⠀⠙⢿⣷⡀⠀⠉⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⣰⣿⡟⠀⠀⠀⠀⠀⠀⣾⣿⠃⣼⡇⠀⢸⡇⠀⠀⠀⠀⠀⠀⣿⠀⢸⣿⣷⡀⠀⢀⣾⣿⡤⠐⠊⢻⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⢠⣿⣿⣼⡇⠀⠀⠀⠀⠀⢠⣿⠉⢠⣿⠧⠀⣸⣇⣠⡄⠀⠀⠀⠀⣿⠠⢸⡟⠹⣿⡍⠉⣿⣿⣧⠀⠀⠀⠻⣿⣶⣄⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⢸⣿⣿⡟⠀⠀⠀⠀⠀⣼⡏⢠⡿⣿⣦⣤⣿⡿⣿⡇⠀⠀⠀⢸⡿⠻⣿⣧⣤⣼⣿⡄⢸⡿⣿⡇⠀⠀⢠⣌⠛⢿⣿⣶⣤⣤⣄⡀\n"
"⠀⠀⠀⣀⣤⣿⣿⠟⣀⠀⠀⠀⠀⠀⣿⢃⣿⠇⢿⣯⣿⣿⣇⣿⠁⠀⠀⠀⣾⡇⢸⣿⠃⠉⠁⠸⣿⣼⡇⢻⡇⠀⠀⠀⢿⣷⣶⣬⣭⣿⣿⣿⠇\n"
"⣾⣿⣿⣿⣿⣻⣥⣾⡇⠀⠀⠀⠀⠀⣿⣿⠇⠀⠘⠿⠋⠻⠿⠿⠶⠶⠾⠿⠿⠍⢛⣧⣰⠶⢀⣀⣼⣿⣴⡸⣿⠀⠀⠀⠸⣿⣿⣿⠉⠛⠉⠀⠀\n"
"⠘⠛⠿⠿⢿⣿⠉⣿⠁⠀⠀⠀⠀⢀⣿⡿⣶⣶⣶⣤⣤⣤⣀⣀⠀⠀⠀⠀⠀⠀⢀⣭⣶⣿⡿⠟⠋⠉⠀⠀⣿⠀⡀⡀⠀⣿⣿⣿⡆⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⣼⣿⠀⣿⠀⠀⠸⠀⠀⠸⣿⠇⠀⠀⣈⣩⣭⣿⡿⠟⠃⠀⠀⠀⠀⠀⠙⠛⠛⠛⠛⠻⠿⠷⠆⠀⣯⠀⠇⡇⠀⣿⡏⣿⣧⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⢿⣿⡀⣿⡆⠀⠀⠀⠀⠀⣿⠰⠿⠿⠛⠋⠉⠀⠀⢀⣴⣶⣶⣶⣶⣶⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣧⠀⠀⠀⣿⡇⣿⣿⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⢸⣿⡇⢻⣇⠀⠘⣰⡀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⠀⠀⠀⢸⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⠀⠀⠀⣿⣧⣿⡿⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠈⣿⣧⢸⣿⡀⠀⡿⣧⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⣿⡄⠀⠀⠀⣼⡇⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣾⡟⢡⣶⠀⢠⣿⣿⣿⠃⠀⠀\n"
"⠀⠀⠀⠀⠀⠹⣿⣿⣿⣷⠀⠇⢹⣷⡸⣿⣶⣦⣄⣀⡀⠀⠀⠀⣿⡇⠀⠀⢠⣿⠁⣀⣀⣠⣤⣶⣾⡿⢿⣿⡇⣼⣿⢀⣿⣿⠿⠏⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠈⠛⠛⣿⣷⣴⠀⢹⣿⣿⣿⡟⠿⠿⣿⣿⣿⣿⣾⣷⣶⣿⣿⣿⣿⡿⠿⠟⠛⠋⠉⠀⢸⣿⣿⣿⣿⣾⣿⠃⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣿⣦⣘⣿⡿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠛⠛⠻⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀\n"
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠻⣿⣿⣿⠈⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";
static const char *memorial =
"⣿⣿⣿⣿⣿⡿⣋⣴⣿⣿⣿⣿⡟⢙⣫⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡭⣿⣾⣿⣿⣿⣏⠣⠙⢿⣿⣿⣿⣿\n"
"⣿⣿⡿⠛⠑⣨⣴⣿⣿⣿⣿⡟⢠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢧⣻⣿⣿⣿⢻⣿⣿⣿⣾⣿⣿⣿⢟⢿⡑⢌⠀⢻⣿⣿⣿\n"
"⣟⣧⣴⡞⣴⣿⣿⢟⣿⣿⢏⢇⣿⣿⣿⣿⡿⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢮⢷⢹⣿⡟⣧⠛⣟⡻⣿⠿⡟⣻⣯⢊⢿⣄⢿⣆⢻⣿⣿\n"
"⣿⣿⢏⣼⣿⣿⣿⣿⣿⣧⠚⢸⣿⣿⣿⣿⡳⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢸⣌⢎⠈⢿⣮⡑⢹⡤⣭⢾⣧⢻⣿⣷⡄⠻⣎⠿⠄⣻⣿\n"
"⣿⠏⣰⣸⣿⡟⣿⣿⣿⣏⢃⣾⣿⣿⣿⡿⣆⣇⢻⣿⣿⣿⣿⣿⣿⣿⣿⠇⠸⠛⢻⣧⣅⠹⡔⠡⢷⣿⣿⣿⣧⡋⣙⣵⢳⣌⡈⠓⢸⣿\n"
"⣏⣸⡇⣿⣿⠠⢸⡁⣤⡡⢿⣯⢸⣿⣿⡔⢸⣿⡁⢟⣿⣿⣿⣿⣿⣿⣟⢤⣷⠞⡛⢛⡊⠥⠜⢙⠌⣿⣿⣿⣿⡇⣿⠿⠷⠍⣾⣛⣐⣶\n"
"⣼⣿⡯⣿⡟⠂⠸⣧⢻⣿⣦⡉⢿⣿⡿⠃⣾⡿⢗⡘⣯⢿⣿⣿⣿⣿⠘⢞⠕⠋⠀⣀⢀⣀⣀⡀⠀⣈⢽⣿⢿⣧⣣⣿⠀⡀⠙⣛⣿⣿\n"
"⣿⣿⣿⢻⠀⣤⠸⣿⣎⢿⣿⣷⡙⡿⠱⠐⢉⣬⣿⣷⡹⣿⣿⣿⣿⣿⢰⣋⣰⣮⣾⣥⣤⣤⣌⣁⠀⢰⣿⢻⣇⣿⣿⣿⡀⡍⠷⣤⣙⣛\n"
"⣿⣿⣿⠱⢸⢹⢀⢹⣿⠘⣿⣿⣧⠃⠆⢷⠟⠋⠭⠭⠇⣿⡿⣻⠏⣿⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣠⢸⣿⣾⣿⢸⣿⣿⡇⣷⢘⠜⣿⣿\n"
"⣮⣍⠛⠇⢸⣸⢈⠀⢿⣧⢹⣿⣿⡜⠀⠁⠁⣀⣤⣶⣾⡟⢡⠏⢰⡜⡸⣿⣿⣿⣿⣿⣿⣿⣿⣟⠋⣸⣿⡿⣿⡏⢻⢿⢅⡗⣈⣑⠾⣿\n"
"⢿⣿⣿⣦⣔⠟⢸⡼⢨⣻⡼⣿⣿⣷⠀⠈⣸⣽⣿⣿⢏⣴⡿⣤⠙⣿⣦⣽⣿⣿⣿⣿⣿⣿⡿⠟⠔⢸⠿⣱⠻⣿⣍⣎⢿⢱⡆⠙⡈⠻\n"
"⣷⡝⢿⣿⣿⣧⡀⠉⡏⣬⡄⢻⣿⣿⡆⢺⣿⣿⣿⣿⣿⣿⣧⣿⣷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠣⠞⢰⣿⢰⣿⣿⣿⡎⢸⣿⡗⠼⣧\n"
"⣿⣿⣦⡙⢿⣿⣿⣦⡡⢸⢰⠸⣿⣿⣧⢮⣻⢿⣿⣿⣿⣿⣿⣿⠿⣻⣿⣿⣿⡿⣫⣽⣿⣿⣿⣥⣶⠃⢸⠇⣸⣿⡟⣿⢹⠘⢟⣼⣷⡉\n"
"⣿⣿⣿⣿⣶⡝⢿⣿⣷⣀⢸⡆⣿⣿⣿⡈⢢⣴⣻⣿⣿⣿⣿⣭⡔⣿⣿⣿⡿⣱⣿⣿⣿⣿⣿⣿⣿⠀⡸⢀⣿⣿⣿⣿⡎⡇⠈⢿⣿⣷\n"
"⣿⣿⣿⠿⣫⡴⠈⢿⣿⣿⣷⡈⣿⣿⣿⣧⠀⠻⠿⣿⣿⣿⣿⣿⣿⣎⣛⣻⣿⣿⣿⣿⣿⣿⣿⣿⢟⠈⠇⣾⣿⣿⣿⣿⡇⢠⠀⡈⣿⣿\n"
"⢿⣿⠵⠞⢉⣴⠗⠘⣿⣿⡿⡁⣿⣿⣿⡿⢣⣾⣿⣶⣝⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢟⣵⣿⢀⣾⣿⣿⣿⢿⣿⡇⠀⡀⠀⣿⣿\n"
"⣼⠷⠖⢂⡿⢡⣴⣾⣶⣄⡑⢼⢟⡆⡿⠱⣿⣿⣿⢿⣿⡇⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣱⣿⠟⡛⣾⡿⣿⠇⡿⣾⣿⡇⠀⠃⠀⣿⣿\n"
"⢈⣐⣤⣿⡇⢸⡿⠿⣿⣿⣿⣦⡈⡌⣠⡷⣸⡿⠊⢈⣿⢀⢳⣄⠈⠛⠿⢿⠿⣿⣛⣵⣿⡿⢁⠐⣼⡿⢹⡿⣼⢣⣿⣿⡇⠠⠀⣀⣛⣛\n"
"⣿⠟⢉⠝⣠⣾⣷⣶⣬⣙⠻⣿⣻⠆⢉⣚⣩⣶⣿⣿⡇⣄⢣⠹⢸⡄⠸⣿⣿⣿⣿⣿⡟⢰⠃⣼⡿⢁⡾⡠⠡⢾⡿⡫⠀⣵⣾⣿⣿⣿\n"
"⣢⢂⣂⠀⠘⠻⠛⠿⢿⣿⣿⢦⡇⠸⠆⣿⣿⣿⣿⣿⡇⣿⣧⢣⠘⠏⡀⠹⣿⣿⣿⣿⠁⣣⣾⣿⠣⠞⠕⡡⣂⠍⡪⢂⣾⣿⣿⣿⣿⣿\n"
"⠏⢿⣿⣿⣷⡐⣦⣽⡶⣮⣭⣜⣃⣠⣿⣿⣿⣿⣿⣿⢃⣿⣿⣧⡀⠀⣾⡆⠹⣿⢿⠅⠰⣿⡿⢇⠆⡀⣼⡗⠁⢰⢡⣿⣿⣿⣿⣿⣿⣿\n"
"⣿⣿⣿⣿⣿⣶⡜⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⡜⣝⢿⣿⡇⠀⢉⣅⠀⣿⣿⢃⡴⣫⡎⣾⢀⢸⢫⠁⠀⣿⠈⣿⣿⣿⣿⣿⣿⣿\n"
"⣿⣿⣿⣿⣿⣿⣿⡤⣶⣶⣶⣬⣹⠿⣻⣧⣿⣿⣷⢠⡏⢫⢢⠏⣸⣿⠿⡠⠊⣴⣿⠫⣽⣿⣿⡇⠸⣧⠎⣷⣄⢹⣿⠈⣿⣿⣿⣿⣿⣿\n"
"⣜⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣗⣞⣰⡧⢂⣼⡿⢃⢎⡾⣼⣿⡅⢰⣿⣿⣿⣷⠀⢻⣰⢹⡌⢳⡝⢀⣿⣿⣿⣿⣿⣿\n";
void ui_banner(bool easter){
    printf("%s",cc(C_MAG)); fputs(easter?memorial:berry,stdout); printf("%s",cc(C_RST));
    printf("%s iBreakPoint %s%s\n",cc(C_MAG),IBP_VERSION,cc(C_RST));
    printf("%s Embedded Firmware Robustness Testing %s\n",cc(C_GREY),cc(C_RST));
    printf("%s────────────────────────────────────────────────────────%s\n",cc(C_GREY),cc(C_RST));
    if(easter){ printf("\n%sRayka L, Miss u So Much, and love u 4ever. Rest In Power my Girl - Rafis%s\n\n",cc(C_GRN),cc(C_RST)); }
}
void ui_menu(void){
    puts("\n [1] Device       Inspect connected hardware");
    puts(" [2] DocTor Who   Inspect device and firmware");
    puts(" [3] Analyze      Check environment readiness");
    puts(" [H] Help         Show commands");
    puts(" [A] About        Project information");
    puts(" [Q] Quit");
}
void ui_about(void){
    ui_heading("About iBreakPoint");
    puts("iBreakPoint is a C-based toolkit for device diagnostics and future controlled firmware robustness testing.");
    puts("Version 0.1.0 focuses on serial identification, a versioned diagnostic protocol, and environment readiness.");
    puts("Target: Raspberry Pi Pico / RP2040 | Host: Fedora Linux");
    puts("\nCreated by: RafisOnRails \nPwnBy:      Akai Ito");
    puts("\nDigite o código de acesso ou pressione Enter para voltar.");
}
