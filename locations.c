#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

static const char *LOCAL_SHARE_SUFFIX = "/.local/share";
static const char *XDG_DATA_HOME = "XDG_DATA_HOME";
static const char *XDG_DATA_DIRS = "XDG_DATA_DIRS";
static const char *HOME = "HOME";
static const char *APPLICATIONS_SUFFIX = "/applications";

static char *get_default_data_dirs(void)
{
    return strdup("/usr/local/share:/usr/share");
}

static char *get_data_dirs_raw(void)
{
    char *data_dirs = getenv(XDG_DATA_DIRS);
    if (data_dirs == NULL)
    {
        return get_default_data_dirs();
    }
    return strdup(data_dirs);
}

static char *get_default_data_home(const char *home)
{
    size_t length = strlen(home) + strlen(LOCAL_SHARE_SUFFIX) + 1;
    char *result = malloc(length);
    if (result)
    {
        strcpy(result, home);
        strcat(result, LOCAL_SHARE_SUFFIX);
    }
    return result;
}

static char *get_data_home(void)
{
    char *home = getenv(HOME);
    if (home == NULL)
    {
        return NULL;
    }
    char *data_home = getenv(XDG_DATA_HOME);
    if (data_home == NULL)
    {
        return get_default_data_home(home);
    }
    return strdup(data_home);
}

static char *get_possible_app_locations_raw(void)
{
    char *data_home = get_data_home();
    char *data_dirs = get_data_dirs_raw();

    char *dirs = malloc(strlen(data_home) + strlen(data_dirs) + 2);
    if (dirs == NULL)
    {
        return NULL;
    }

    strcpy(dirs, data_home);
    strcat(dirs, ":");
    strcat(dirs, data_dirs);
    printf("dirs: %s\n", dirs);

    free(data_home);
    printf("data_dirs: %s\n", data_dirs);
    free(data_dirs);
    return dirs;
}

char** get_possible_app_locations(void)
{
    char *dirs = get_possible_app_locations_raw();
    size_t count = 0;
    char *temp = strdup(dirs);
    char *saveptr;

    char *token = strtok_r(temp, ":", &saveptr);
    while (token != NULL)
    {
        count++;
        token = strtok_r(NULL, ":", &saveptr);
    }
    free(temp);
    
    char **locations = malloc((count + 1) * sizeof(char *));
    if (locations == NULL)
    {
        free(dirs);
        return NULL;
    }

    count = 0;
    token = strtok_r(dirs, ":", &saveptr);
    while (token != NULL)
    {
        char *location = malloc(strlen(token) + strlen(APPLICATIONS_SUFFIX) + 1);
        strcpy(location, token);
        strcat(location, APPLICATIONS_SUFFIX);

        locations[count++] = location;

        token = strtok_r(NULL, ":", &saveptr);
    }
    locations[count] = NULL;

    free(dirs);
    return locations;
}
