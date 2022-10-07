#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <bilt/base.h>
#include <bilt/plugin-options.h>
#include <bilt/plugin.h>
#include <bilt/fmt.h>
#include <bilt/plugin-array.h>
#include <bilt/misc/limits.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#ifdef __APPLE__
#define LIBRARY_EXTENSION ".dylib"
#elif defined(_WIN32)
#define LIBRARY_EXTENSION ".dll"
#else
#define LIBRARY_EXTENSION ".so"
#endif


// https://stackoverflow.com/a/10347734
int filename_has_extension(char *filename, const char *extension) {
  char *dot = strchr (filename, '.');
  if(dot && !strcmp(dot, extension)) {
    return 1;
  }
  return 0;
}

char *bilt_plugin_path_from_name(char *name) {
  DIR *d_plugins;
  struct dirent *dir;
  char *result = NULL;
  int found_result = 0;
  d_plugins = opendir (PLUGINS_DIR);
  if(!d_plugins) {
    bilt_err ("Error opening plugin directory " PLUGINS_DIR "\n");
    return NULL;
  }
  while((dir = readdir(d_plugins)) != NULL && !found_result) {
    if(dir->d_type == DT_REG) {
      if(filename_has_extension (dir->d_name, LIBRARY_EXTENSION)) {
        char filename[PATH_MAX];
        sprintf(filename, "%s/%s", PLUGINS_DIR, dir->d_name);
        void *plugin_handle = dlopen (filename, RTLD_LAZY | RTLD_LOCAL);
        const char **plugin_name = dlsym (plugin_handle, "PLUGIN_NAME");
        if(!strcmp(*plugin_name, name)) {
          found_result = 1;
          result = malloc(PATH_MAX * sizeof(char));
          strcpy (result, filename);
        }
        dlclose(plugin_handle);
      }
    }
  }
  closedir(d_plugins);
  return result;
}
char *bilt_plugin_name_from_path(char *path) {
  char *result = malloc(256 * sizeof(char));
  void *plugin = dlopen (path, RTLD_LAZY | RTLD_LOCAL);
  if(!plugin) {
    return NULL;
  }
  char **plugin_name = dlsym (plugin, "PLUGIN_NAME");
  strcpy(result, *plugin_name);
  dlclose(plugin);
  return result;
}

struct bilt_plugin_trigger *bilt_plugin_should_run(void *plugin, int argc, char **argv) {
  struct bilt_plugin_trigger *triggers = bilt_plugin_get_triggers(plugin);
  struct bilt_plugin_trigger *triggered_by = NULL;
  struct bilt_plugin_trigger *curTrigger = triggers;
  while(curTrigger->type != TYPE_TERMINATE_LIST && !triggered_by) {
    switch(curTrigger->type) {
    case TYPE_FILE: {
      FILE *expected_file = fopen(curTrigger->data, "r");
      if(expected_file != NULL) {
        triggered_by = curTrigger;
        fclose(expected_file);
      }
      break;
    }
    case TYPE_COMMAND: {
      if(argc > 1 && !strcmp(argv[1], curTrigger->data)) {
        triggered_by = curTrigger;
      }
      break;
    }
    case TYPE_TERMINATE_LIST: break;
    }
    curTrigger++;
  }
  return triggered_by;
}

int bilt_plugin_run(void *plugin, struct bilt_plugin_trigger trigger, int argc, char **argv) {
  int (*plugin_main)(struct bilt_plugin_trigger, int, char **) = dlsym (plugin, "plugin_main");
  return plugin_main (trigger, argc, argv);
}

int bilt_plugin_run_path(char *dlpath, struct bilt_plugin_trigger trigger, int argc, char **argv) {
  void *plugin = dlopen (dlpath, RTLD_LAZY | RTLD_LOCAL);
  int result = bilt_plugin_run (plugin, trigger, argc, argv);
  dlclose (plugin);
  return result;
}

int bilt_plugin_run_name(char *name, struct bilt_plugin_trigger trigger, int argc, char **argv) {
  char *path = bilt_plugin_path_from_name(name);
  int result = bilt_plugin_run_path(path, trigger, argc, argv);
  free (path);
  return result;
}

char **bilt_plugin_get_name(void *plugin) {
  return dlsym(plugin, "PLUGIN_NAME");
}
char **bilt_plugin_get_version(void *plugin) {
  return dlsym(plugin, "PLUGIN_VERSION");
}
struct bilt_plugin_trigger *bilt_plugin_get_triggers(void *plugin) {
  return dlsym(plugin, "PLUGIN_TRIGGERS");
}

bilt_plugin_array bilt_plugin_get_all() {
  DIR *d_plugins;
  bilt_plugin_array arr;
  bilt_plugin_array_init (&arr);
  struct dirent *dir;
  d_plugins = opendir (PLUGINS_DIR);
  if(!d_plugins) {
    bilt_err ("Error opening plugin directory " PLUGINS_DIR "\n");
    return arr;
  }
  while((dir = readdir(d_plugins)) != NULL) {
    if(dir->d_type == DT_REG) {
      if(filename_has_extension (dir->d_name, LIBRARY_EXTENSION)) {
        char filename[PATH_MAX];
        sprintf(filename, "%s/%s", PLUGINS_DIR, dir->d_name);
        void *plugin_handle = dlopen (filename, RTLD_LAZY | RTLD_LOCAL);
        bilt_plugin_array_add (&arr, plugin_handle);
      }
    }
  }
  closedir(d_plugins);
  return arr;
}
