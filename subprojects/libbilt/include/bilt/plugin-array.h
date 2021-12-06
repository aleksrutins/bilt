// https://stackoverflow.com/a/3536261
#pragma once
#include <stddef.h>
typedef struct {
  void **plugins;
  size_t used;
  size_t allocated;
} bilt_plugin_array;

void bilt_plugin_array_init(bilt_plugin_array *array);
void bilt_plugin_array_add(bilt_plugin_array *array, void *plugin);
void bilt_plugin_array_unload(bilt_plugin_array *array, size_t index);
