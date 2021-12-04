struct bilt_plugin_trigger {
  enum bilt_plugin_trigger_type {
    TYPE_FILE,
    TYPE_COMMAND,
    TYPE_TERMINATE_LIST
  } type;
  const char *data;
};
#define bilt_plugin_name(name) const char *PLUGIN_NAME = #name
#define bilt_plugin_version(version) const char *PLUGIN_VERSION = #version
#define bilt_plugin_triggers_begin const struct bilt_plugin_trigger PLUGIN_TRIGGERS[] = {
#define bilt_command_trigger(cmd) { TYPE_COMMAND, #cmd },
#define bilt_file_trigger(file) { TYPE_FILE, file },
#define bilt_plugin_triggers_end { TYPE_TERMINATE_LIST, NULL } \
};
