#include "./path_handler.h"
#include <stdlib.h>
#include <string.h>


char *get_file_extension(char* path) {
    char* cut_point = NULL;

    char* last_dot = strrchr(path, '.');
    char* last_slash = strrchr(path, '/');
    char* last_backslash = strrchr(path, '\\');
    char* last_separator = (last_slash > last_backslash) ? last_slash : last_backslash;

    if (last_dot != NULL) {
        // There's a dot

        if (last_separator != NULL) {
            // There's a dot and separator

            if (last_dot > last_separator) {
                // File might be hidden and might have extension
            
                if (last_separator + 1 != last_dot) {
                    cut_point = last_dot + 1; // file might be hidden, but for sure have extension
                }
            }
        }
        else {
            // There's a dot without separator

            if (last_dot != path) {
                cut_point = last_dot + 1; // File is not hidden, there's extension
            }
        }
    }

    if (cut_point == NULL) {
        cut_point = path + (strlen(path));
    }

    return strdup(cut_point);
}

char *remove_file_extension(const char* path) {
    char* clean_path;
    
    clean_path = strdup(path);
    if (clean_path == NULL) {
        return NULL;
    }

    char* last_dot = strrchr(clean_path, '.');
    char* last_slash = strrchr(clean_path, '/');
    char* last_backslash = strrchr(clean_path, '\\');
    char* last_separator = (last_slash > last_backslash) ? last_slash : last_backslash;

    if (last_dot != NULL) {
        // There's a dot

        if (last_separator != NULL) {
            // There's a dot and separator

            if (last_dot > last_separator) {
                // File might be hidden and might have extension
            
                if (last_separator + 1 != last_dot) {
                    *last_dot = '\0'; // file might be hidden, but for sure have extension
                }
            }
        }
        else {
            // There's a dot without separator

            if (last_dot != clean_path) {
                *last_dot = '\0'; // File is not hidden, there's extension
            }
        }
    }

    return clean_path;
}

char *add_file_extension(char *path, char *extension) {
    char *full_path;
    size_t path_len;
    size_t ext_len;

    if (path == NULL || extension == NULL) {
        return NULL;
    }

    path_len = strlen(path);
    ext_len = strlen(extension);

    // Add space for dot, extension and \0
    full_path = malloc(sizeof(char) * path_len + ext_len + 2);
    if (full_path == NULL) {
        return NULL;
    }
    
    strcpy(full_path, path);
    strcat(full_path, ".");
    strcat(full_path, extension);

    return full_path;
}