#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "locations.h"
#include "app.h"

#define MAX_PATH 1024
#define MAX_LINE 512

int starts_with(const char *line, const char *prefix)
{
    return strncmp(line, prefix, strlen(prefix)) == 0;
}

App *parse_desktop_file(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (!file)
    {
        perror("Error opening file");
        return NULL;
    }

    char line[MAX_LINE];
    char name[MAX_LINE] = {0};
    char exec[MAX_LINE] = {0};
    int no_display = 0, terminal = 0, hidden = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (starts_with(line, "Name=") && !name[0])
        {
            strncpy(name, line + 5, sizeof(name) - 1);
            name[strcspn(name, "\n")] = '\0';
        }
        else if (starts_with(line, "Exec=") && !exec[0])
        {
            strncpy(exec, line + 5, sizeof(exec) - 1);
            exec[strcspn(exec, "\n")] = '\0';
        }
        else if (starts_with(line, "NoDisplay="))
        {
            no_display = strstr(line, "true") != NULL;
        }
        else if (starts_with(line, "Terminal="))
        {
            terminal = strstr(line, "true") != NULL;
        }
        else if (starts_with(line, "Hidden="))
        {
            hidden = strstr(line, "true") != NULL;
        }
        else if (starts_with(line, "Type="))
        {
            if (strstr(line, "Application") == NULL)
            {
                no_display = 1;
            }
        }

        if (no_display || terminal || hidden)
        {
            break;
        }
    }

    fclose(file);

    if (no_display || terminal || hidden || !name[0] || !exec[0])
    {
        return NULL;
    }

    for (char *p = exec; *p; ++p)
    {
        if (*p == '%' && *(p + 1))
        {
            *p = '\0';
            break;
        }
    }

    App *app = malloc(sizeof(App));
    if (app)
    {
        strncpy(app->name, name, sizeof(app->name) - 1);
        strncpy(app->exec, exec, sizeof(app->exec) - 1);
    }
    
    return app;
}

App **search_desktop_files(const char *dir_path)
{
    DIR *dir = opendir(dir_path);
    if (!dir)
    {
        fprintf(stderr, "Error opening directory '%s': %s\n", dir_path, strerror(errno));
        return NULL;
    }

    struct dirent *entry;
    char full_path[MAX_PATH];

    App **discoverd_apps = NULL;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        if (entry->d_type == DT_DIR)
        {
            App **app = search_desktop_files(full_path);
            if (app)
            {
                discoverd_apps = combine_apps(discoverd_apps, app);
            }
        }
        else if (strstr(entry->d_name, ".desktop"))
        {
            App *app = parse_desktop_file(full_path);
            if (app)
            {
                discoverd_apps = append_app(discoverd_apps, app);
            }
        }
    }

    closedir(dir);
    return discoverd_apps;
}

int main()
{
    App **discoverd_apps = NULL;
    char **locations;
    
    locations = get_possible_app_locations();
    for (int i = 0; locations[i] != NULL; i++)
    {
        App **apps = search_desktop_files(locations[i]);
        if (apps)
        {
            discoverd_apps = combine_apps(discoverd_apps, apps);
        }
    }
    for (int i = 0; discoverd_apps[i] != NULL; i++)
    {
        printf("apps[%d] -> Name: %s | Exec: %s\n", i, discoverd_apps[i]->name, discoverd_apps[i]->exec);
    }

    return 0;
}
