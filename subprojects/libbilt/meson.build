project('libbilt', 'c',
          version: '0.1.0',
    meson_version: '>= 0.54.0',
  default_options: [ 'warning_level=2',
                     'c_std=gnu11',
                   ],
)
libbilt_include = include_directories('include')
subdir('src')
libbilt_dep = declare_dependency(link_with: libbilt_lib, include_directories: libbilt_include)
meson.override_dependency('libbilt', libbilt_dep)
pkg = import('pkgconfig')

pkg.generate(
	libraries: [libbilt_lib],
	subdirs: ['bilt'],
	version: meson.project_version(),
	name: 'libbilt',
	filebase: 'libbilt',
	description: 'Support library for bilt and bilt plugins'
)
