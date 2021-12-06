#pragma once
#include "plugin-array.h"
typedef void * bilt_plugin;
char *bilt_plugin_path_from_name(char *name);
struct bilt_plugin_trigger *bilt_plugin_should_run(bilt_plugin plugin, int argc, char **argv);
int bilt_plugin_run(bilt_plugin plugin, struct bilt_plugin_trigger trigger, int argc, char **argv);
int bilt_plugin_run_path(char *dlpath, struct bilt_plugin_trigger trigger, int argc, char **argv);
int bilt_plugin_run_name(char *name, struct bilt_plugin_trigger trigger, int argc, char **argv);
char **bilt_plugin_get_name(bilt_plugin plugin);
char **bilt_plugin_get_version(bilt_plugin plugin);
struct bilt_plugin_trigger *bilt_plugin_get_triggers(bilt_plugin plugin);
bilt_plugin_array bilt_plugin_get_all();
