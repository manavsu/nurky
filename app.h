#ifndef APP_H
#define APP_H

#define MAX_PATH 1024
#define MAX_LINE 512

typedef struct
{
    char name[MAX_LINE];
    char exec[MAX_LINE];
} App;

App **append_app(App **apps, App *app); // TODO: Pass in count here
App **combine_apps(App **pps1, App **apps2);

#endif