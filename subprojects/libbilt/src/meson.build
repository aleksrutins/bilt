config_h = configuration_data()
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())

if get_option('plugins_dir') == 'auto'
	config_h.set_quoted('PLUGINS_DIR', get_option('prefix') / 'share/bilt/plugins')
else
	config_h.set_quoted('PLUGINS_DIR', get_option('plugins_dir'))
endif

configure_file(
  output: 'config.h',
  configuration: config_h,
)

deps = [
	meson.get_compiler('c').find_library('dl')
]

libbilt_lib = shared_library('bilt', [
	'main.c',
	'plugin.c',
	'plugin-array.c'
], install: true, include_directories: libbilt_include, dependencies: deps)
