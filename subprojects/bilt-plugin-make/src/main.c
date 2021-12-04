#include <stdio.h>
#include <bilt/base.h>
#include <bilt/plugin-options.h>
#include <bilt/fmt.h>
#include <stdlib.h>
#include <string.h>

bilt_plugin_name (make);
bilt_plugin_version (0.1.0);
bilt_plugin_triggers_begin
  bilt_file_trigger ("makefile")
  bilt_file_trigger ("Makefile")
  bilt_command_trigger (make)
bilt_plugin_triggers_end



int plugin_main(struct bilt_plugin_trigger trigger, int argc, char **argv) {
  if(trigger.type == TYPE_COMMAND) { // Initialize a project
    bilt_info("Initializing Makefile...");
    char makefile_name[sizeof("makefile")];
    bilt_prompt ("Makefile name (either 'Makefile' or 'makefile')", makefile_name, sizeof (makefile_name));
    bilt_info("Writing...");
    FILE *makefile = fopen(makefile_name, "w+");
    if(makefile == NULL) {
      bilt_err("Error writing makefile");
      return 1;
    }
    fputs(
".PHONY: all\n"
"all:\n"
"\techo 'Put your build script here!'\n"
    , makefile);
    fclose(makefile);
    bilt_info("Done!");
    return 0;
  }
  return system("make");
}
