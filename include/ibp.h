#ifndef IBP_H
#define IBP_H
#include <stdbool.h>
#include <stddef.h>
#define IBP_VERSION "0.1.0"
#define IBP_PROTOCOL_VERSION "1"
#define IBP_LINE_MAX 512
extern int ibp_color_enabled;
void ui_init(const char *mode);
void ui_banner(bool easter);
void ui_menu(void);
void ui_about(void);
void ui_info(const char *s);
void ui_ok(const char *s);
void ui_warn(const char *s);
void ui_fail(const char *s);
void ui_heading(const char *s);
int cmd_device_list(void);
int cmd_device_info(const char *path);
int cmd_doctor(const char *action, const char *path);
int cmd_analyze(const char *path);
int protocol_parse_line(const char *line, char *kind, size_t kind_n, char *payload, size_t payload_n);
int serial_request(const char *path, const char *request, char *response, size_t response_n, int timeout_ms, double *latency_ms);
const char *default_serial_device(void);
void print_help(void);
#endif
