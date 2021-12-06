#include <stdio.h>
#include <bilt/base.h>
#include <bilt/plugin-options.h>
#include <bilt/fmt.h>
#include <stdlib.h>
#include <string.h>

bilt_plugin_name (generator);
bilt_plugin_version (0.1.0);
bilt_plugin_triggers_begin
  bilt_command_trigger (gen)
bilt_plugin_triggers_end



int plugin_main(struct bilt_plugin_trigger trigger, int argc, char **argv) {
  if(argc < 3) {
    bilt_err ("Usage: bilt gen <template> <target-directory>");
    return 1;
  }
  return 0;
}
