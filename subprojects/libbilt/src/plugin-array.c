#include <bilt/plugin-array.h>
#include <bilt/plugin.h>
#include <stdlib.h>
#include <dlfcn.h>

// https://stackoverflow.com/a/3536261

void bilt_plugin_array_init(bilt_plugin_array *array) {
  array->plugins = malloc(2 * sizeof (bilt_plugin));
  array->used = 0;
  array->allocated = 2;
}

void bilt_plugin_array_add(bilt_plugin_array *array, bilt_plugin plugin) {
  if(array->used == array->allocated) {
    array->allocated += 2;
    array->plugins = realloc (array->plugins, array->allocated * sizeof(bilt_plugin));
  }
  array->plugins[array->used++] = plugin;
}
void bilt_plugin_array_unload(bilt_plugin_array *array, size_t index) {
  if(index >= array->used) return;
  dlclose(array->plugins[index]);
  array->plugins[index] = NULL;
}
