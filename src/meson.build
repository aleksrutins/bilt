bilt_sources = [
  'main.c',
]

bilt_deps = [
  dependency('lua', required: get_option('use_lua')),
  dependency('libbilt')
]

executable('bilt', bilt_sources,
  dependencies: bilt_deps,
  install: true,
)
