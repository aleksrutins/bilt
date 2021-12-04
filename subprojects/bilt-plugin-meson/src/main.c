#include <stdio.h>
#include <bilt/base.h>
#include <bilt/plugin-options.h>
#include <bilt/fmt.h>
#include <bilt/cmd.h>
#include <string.h>

bilt_plugin_name (meson);
bilt_plugin_version (0.1.0);
bilt_plugin_triggers_begin
  bilt_file_trigger ("meson.build")
  bilt_command_trigger (meson)
bilt_plugin_triggers_end

int plugin_main(struct bilt_plugin_trigger trigger, int argc, char **argv) {
  bilt_info ("Configuring...");
  bilt_run_cmd ("meson builddir");
  bilt_info("Building...");
  bilt_run_cmd("meson compile -C builddir");
  if(argc > 1 && !strcmp(argv[1], "--install")) {
    bilt_info("Installing... (will require sudo)");
    bilt_run_cmd ("sudo meson install -C builddir");
  }
  return 0;
}
