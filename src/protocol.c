#include "ibp.h"
#include <string.h>
#include <ctype.h>
int protocol_parse_line(const char *line, char *kind, size_t kind_n, char *payload, size_t payload_n) {
    const char *p;
    size_t n=0;
    if(!line || !kind || !payload || kind_n<2 || payload_n<1) return -1;
    if(strncmp(line,"IBP/1 ",6)!=0) return -1;
    p=line+6;
    while(p[n] && !isspace((unsigned char)p[n]) && n<kind_n-1) n++;
    if(n==0 || (p[n] && !isspace((unsigned char)p[n]))) return -1;
    memcpy(kind,p,n); kind[n]='\0';
    p+=n; while(*p==' ' || *p=='\t') p++;
    n=strcspn(p,"\r\n");
    if(n>=payload_n) return -1;
    memcpy(payload,p,n); payload[n]='\0';
    return 0;
}
