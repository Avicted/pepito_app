project('pépito_app', 'cpp',
  version : '0.1.0',
  default_options : [
    'cpp_std=c++23',
    'warning_level=3', # 3 is the default
    'buildtype=release',
    'optimization=2',
  ]
)

cc = meson.get_compiler('cpp')
cxx = meson.get_compiler('cpp')

c_args = [
  
]
cpp_args = [
  
  '-Wno-pedantic', 
  '-Wno-narrowing',
  '-Wno-missing-field-initializers',
]
link_args = [
    # '-lraylib',
    '-lcurl',
]
name_suffix = []

# Find the raylib dependency
# raylib_dep = dependency('raylib')

dependencies = [
    # raylib_dep,
]

# Specify the src and includes directories
src_dir = 'src'
inc_dir = include_directories(
    'includes'
)

# Project files
# Collect all .cpp files in the src directory
src_files = files(
    'src/Main.cpp',
    'src/ServerSideEventsClient.cpp',
)

# Collect all .hpp files in the includes directory
header_files = files(
    'includes/Includes.hpp',
    'includes/ServerSideEventsClient.hpp',
)

# Define the executable
executable('pepito_app', 
  src_files,
  c_args: c_args,
  cpp_args: cpp_args,
  dependencies: dependencies,
  include_directories: inc_dir,
  link_args: link_args,
  name_suffix: name_suffix,
)

# Optionally, you can specify install locations for headers
install_headers(header_files, subdir: 'pepito_app')
