# bilt
Bilt is a build system wrapper. The end goal is to have soomething where you can navigate into a project, type `bilt`, and then have it automatically built (and possibly installed) for you.
It works on a system of plugins, where each plugin is a shared library in the `(prefix)/share/bilt/plugins` directory.

Currently, only Linux is supported, as it uses `PATH_MAX` from `<linux/limits.h>` to stay within OS limits.

## Installation
To install, use Meson:
```sh
meson builddir
meson compile -C builddir
sudo meson install -C builddir
```
Configure options:

**`plugins_dir`**: Directory to put plugins in. By default, this is `(prefix)/share/bilt/plugins`. Make sure to use this same value for other plugins that you're installing!

## Usage
Just type `bilt`. Some plugins may have extra commands or flags that they provide. Docs for the built-in ones coming soon.

## Built-in plugins
Currently, the built-in plugins are:
- [make](subprojects/bilt-plugin-make)
- [meson](subprojects/bilt-plugin-meson)

Planned:
- Gradle
- Android Gradle
- CMake

## Writing your own plugin
Copy one of the already written plugins into your own directory, and customize the code. Docs coming soon.
