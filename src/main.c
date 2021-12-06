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
#include <bilt/fmt.h>
#include <bilt/plugin.h>
#include <bilt/plugin-array.h>
#include <bilt/plugin-options.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>



int
main (int   argc,
      char *argv[])
{
  char cwd[PATH_MAX];
  char run_only_command[256] = "";
  bilt_plugin run_only_plugin = NULL;
  getcwd(cwd, PATH_MAX);
  printf("\e[96mcwd\e[0m\t%s\n", cwd);
  puts("\e[96mscan\e[0m\t" PLUGINS_DIR);

  bilt_plugin_array plugins = bilt_plugin_get_all();
  for(register size_t i = 0; i < plugins.used; i++) {
    bilt_plugin plugin = plugins.plugins[i];
    printf("\e[36mplugin\e[0m\t\e[1m%s\e[0m version \e[1m%s\e[0m\n", *bilt_plugin_get_name (plugin), *bilt_plugin_get_version (plugin));
    struct bilt_plugin_trigger *trigger = bilt_plugin_should_run (plugin, argc, argv);
    if(!trigger) {
      bilt_plugin_array_unload (&plugins, i);
      printf("\tnot running\n");
    } else {
      char trigger_type_str[] = "unknown";
      switch(trigger->type) {
      case TYPE_FILE: {
        strcpy (trigger_type_str, "file");
        break;
      }
      case TYPE_COMMAND: {
        strcpy (trigger_type_str, "command");
        strncpy (run_only_command, trigger->data, 256);
        run_only_plugin = plugin;
        break;
      }
      default: break;
      }
      printf("\t\e[1mrunning\e[0m trigger %s \e[1m%s\e[0m\n", trigger_type_str, trigger->data);
    }
  }
  if(strcmp(run_only_command, "") && run_only_plugin) {
    printf("\n\e[36minfo\e[0m\trunning only plugin \e[1m%s\e[0m due to command \e[1m%s\e[0m\n", *bilt_plugin_get_name (run_only_plugin), run_only_command);
  }
  // Run plugins
  for(register size_t i = 0; i < plugins.used; i++) {
    bilt_plugin plugin = plugins.plugins[i];
    if(plugin && !(run_only_plugin && run_only_plugin != plugin)) {
      printf("\n\e[1;32mrun\e[0m\tplugin \e[1m%s\e[0m\n", *bilt_plugin_get_name (plugin));
      int plugin_argc = argc;
      char **plugin_argv = argv;
      struct bilt_plugin_trigger *trigger = bilt_plugin_should_run (plugin, argc, argv);
      if(trigger->type == TYPE_COMMAND) {
        plugin_argc--;
        plugin_argv++;
      }
      bilt_plugin_run (plugin, *trigger, plugin_argc, plugin_argv);
    }
    if(plugin) bilt_plugin_array_unload (&plugins, i);
  }

}
