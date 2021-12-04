/* main.c
 *
 * Copyright 2021 Aleks Rutins
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bilt-config.h"

#include <stdlib.h>
#include <stdio.h>
#include <bilt/base.h>
#include <bilt/plugin-options.h>
#include <bilt/fmt.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>

struct plugin_data {
  char *name;
  char *version;
  struct bilt_plugin_trigger *triggers;
};

// https://stackoverflow.com/a/10347734
int filename_has_extension(char *filename, const char *extension) {
  char *dot = strchr (filename, '.');
  if(dot && !strcmp(dot, extension)) {
    return 1;
  }
  return 0;
}

int
main (int   argc,
      char *argv[])
{
  int result = 0;
  DIR *d_plugins;
  struct dirent *dir;
  d_plugins = opendir (PLUGINS_DIR);
  if(!d_plugins) {
    bilt_err ("Error opening plugin directory " PLUGINS_DIR "\n");
    return 1;
  }
  char cwd[PATH_MAX];
  getcwd(cwd, PATH_MAX);
  printf("\e[96mcwd\e[0m\t%s\n", cwd);
  puts("\e[96mscan\e[0m\t" PLUGINS_DIR);
  while((dir = readdir(d_plugins)) != NULL) {
    if(dir->d_type == DT_REG) {
      if(filename_has_extension (dir->d_name, ".so")) {
        char filename[PATH_MAX];
        sprintf(filename, "%s/%s", PLUGINS_DIR, dir->d_name);
        void *plugin_handle = dlopen (filename, RTLD_LAZY | RTLD_LOCAL);
        int (*plugin_main)(struct bilt_plugin_trigger, int, char **) = dlsym (plugin_handle, "plugin_main");
        const char **plugin_name = dlsym (plugin_handle, "PLUGIN_NAME");
        const char **plugin_version = dlsym (plugin_handle, "PLUGIN_VERSION");
        const struct bilt_plugin_trigger *plugin_triggers = dlsym(plugin_handle, "PLUGIN_TRIGGERS");
        printf("\e[36mplugin\e[0m\t\e[1m%s\e[0m version \e[1m%s\e[0m\n", *plugin_name, *plugin_version);

        int should_run = 0;
        const struct bilt_plugin_trigger *triggered_by = NULL;
        const struct bilt_plugin_trigger *curTrigger = plugin_triggers;
        while(curTrigger->type != TYPE_TERMINATE_LIST && !should_run) {
          switch(curTrigger->type) {
          case TYPE_FILE: {
            printf("\e[33mtrigger\e[0m\t\e[34mfile\e[0m %s", curTrigger->data);
            FILE *expected_file = fopen(curTrigger->data, "r");
            if(expected_file != NULL) {
              printf("\t\e[1;32mfound\e[0m");
              should_run = 1;
              triggered_by = curTrigger;
              fclose(expected_file);
            }
            break;
          }
          case TYPE_COMMAND: {
            printf("\e[33mtrigger\e[0m\t\e[96mcommand\e[0m %s", curTrigger->data);
            if(argc > 1 && !strcmp(argv[1], curTrigger->data)) {
              printf("\t\e[1;32mfound\e[0m");
              should_run = 1;
              triggered_by = curTrigger;
            }
            break;
          }
          }
          putc('\n', stdout);
          curTrigger++;
        }
        if(should_run) {
          printf("\e[32mrun\e[0m\t%s\n", *plugin_name);
          int plugin_argc = argc;
          char **plugin_argv = argv;
          if(triggered_by->type == TYPE_COMMAND) { // Pass arguments from subcommand
            plugin_argc = argc - 1;
            plugin_argv = argv + 1;
          }
          int returnval = plugin_main(*triggered_by, plugin_argc, plugin_argv);
          if(returnval > 0) {
            result = returnval;
          }
        }
        dlclose(plugin_handle);
      } else {
        printf("\e[33mfile\e[0m\t%s\n", dir->d_name);
      }
    }
    if(result > 0) {
      break;
    }
  }
  closedir(d_plugins);
  return result;
}
