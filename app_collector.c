#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#define MAX_PATH 1024
#define MAX_LINE 512

// Parse a .desktop file to extract the "Name" and "Exec" fields
void parse_desktop_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE];
    char name[MAX_LINE] = {0};
    char exec[MAX_LINE] = {0};

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Name=", 5) == 0) {
            strncpy(name, line + 5, sizeof(name) - 1);
            name[strcspn(name, "\n")] = '\0'; // Remove trailing newline
        } else if (strncmp(line, "Exec=", 5) == 0) {
            strncpy(exec, line + 5, sizeof(exec) - 1);
            exec[strcspn(exec, "\n")] = '\0'; // Remove trailing newline
        }

        // Stop parsing once both fields are found
        if (name[0] && exec[0]) {
            break;
        }
    }

    fclose(file);

    if (name[0] && exec[0]) {
        printf("Name: %s | Exec: %s\n", name, exec);
    }
}

// Recursively search for .desktop files in a directory
void search_desktop_files(const char *dir_path) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        fprintf(stderr, "Error opening directory '%s': %s\n", dir_path, strerror(errno));
        return;
    }

    struct dirent *entry;
    char full_path[MAX_PATH];

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            // Recursively search subdirectories
            search_desktop_files(full_path);
        } else if (strstr(entry->d_name, ".desktop")) {
            // Parse .desktop file
            parse_desktop_file(full_path);
        }
    }

    closedir(dir);
}

int main() {
    // Standard locations for .desktop files
    const char *locations[] = {
        "/usr/share/applications",
        "/usr/local/share/applications",
        getenv("HOME") ? strcat(getenv("HOME"), "/.local/share/applications") : NULL
    };

    printf("Searching for launchable applications...\n");

    for (int i = 0; i < sizeof(locations) / sizeof(locations[0]); i++) {
        if (locations[i]) {
            search_desktop_files(locations[i]);
        }
    }

    return 0;
}
