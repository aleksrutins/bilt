project('bilt-plugin-meson', 'c', version: '0.1.0', meson_version: '>= 0.50.0')

config_h = configuration_data()
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
plugins_dir = ''
if get_option('plugins_dir') == 'auto'
	plugins_dir = get_option('prefix') / 'share/bilt/plugins'
else
	plugins_dir = get_option('plugins_dir')
endif

configure_file(
  output: 'bilt-meson-config.h',
  configuration: config_h,
)

add_project_arguments([
  '-I' + meson.build_root(),
], language: 'c')

subdir('src')
