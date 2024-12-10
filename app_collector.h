#ifndef APP_COLLECTOR_H
#define APP_COLLECTOR_H

#define MAX_PATH 1024
#define MAX_LINE 512

typedef struct
{
    char name[MAX_LINE];
    char exec[MAX_LINE];
} App;

App **collect_apps(void);

#endif