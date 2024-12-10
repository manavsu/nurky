#include <stdlib.h>

#define MAX_PATH 1024
#define MAX_LINE 512
typedef struct {
    char name[MAX_LINE];
    char exec[MAX_LINE];
} App;

App **append_app(App **apps, App *app)
{
    if (!app)
    {
        return apps;
    }

    if (!apps)
    {
        apps = malloc(2 * sizeof(App *));
        if (!apps)
        {
            return NULL;
        }
        apps[0] = app;
        apps[1] = NULL;
        return apps;
    }

    size_t count = 0;
    while (apps[count] != NULL)
    {
        count++;
    }

    apps = realloc(apps, (count + 2) * sizeof(App *));
    if (!apps)
    {
        return NULL;
    }

    apps[count] = app;
    apps[count + 1] = NULL;
    return apps;
}

App **combine_apps(App **apps_a, App **apps_b)
{
    if (!apps_a)
    {
        return apps_b;
    }

    if (!apps_b)
    {
        return apps_a;
    }

    size_t count = 0;
    while (apps_a[count] != NULL)
    {
        count++;
    }

    size_t count_b = 0;
    while (apps_b[count_b] != NULL)
    {
        count_b++;
    }

    apps_a = realloc(apps_a, (count + count_b + 1) * sizeof(App *));
    if (!apps_a)
    {
        return NULL;
    }

    for (size_t i = 0; i < count_b; i++)
    {
        apps_a[count + i] = apps_b[i];
    }

    apps_a[count + count_b] = NULL;
    free(apps_b);
    return apps_a;
}