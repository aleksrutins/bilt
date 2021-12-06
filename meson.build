project('bilt', 'c',
          version: '0.1.0',
    meson_version: '>= 0.50.0',
  default_options: [ 'warning_level=2',
                     'c_std=gnu11',
                   ],
)


config_h = configuration_data()
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())

if get_option('plugins_dir') == 'auto'
	config_h.set_quoted('PLUGINS_DIR', get_option('prefix') / 'share/bilt/plugins')
else
	config_h.set_quoted('PLUGINS_DIR', get_option('plugins_dir'))
endif

configure_file(
  output: 'bilt-config.h',
  configuration: config_h,
)
add_project_arguments([
  '-I' + meson.build_root(),
], language: 'c')

subproject('libbilt')
subproject('bilt-plugin-meson')
subproject('bilt-plugin-make')
subproject('bilt-plugin-generator')
subdir('src')
