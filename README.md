libUIOHook: Cross-platform keyboard and mouse hooking from userland. 
====================================================================
this keyHook is only compile in exe (unsupport apple)



![build-binaires](https://github.com/kwhat/libuiohook/workflows/build-binaires/badge.svg)

## Compiling
Prerequisites: 
 * [cmake](https://cmake.org)
 * gcc, clang or msvc

```
$ git clone https://github.com/kwhat/libuiohook
$ cd libuiohook
$ mkdir build && cd build
$ cmake -S .. -D BUILD_SHARED_LIBS=ON -D BUILD_DEMO=ON -DCMAKE_INSTALL_PREFIX=../dist
$ cmake --build . --parallel 2 --target install  
```

### Configuration

|           | option                        | description            | default |
| --------- | ----------------------------- | ---------------------- | ------- | 
| __all__   | BUILD_DEMO:BOOL               | demo applications      | OFF     |
|           | BUILD_SHARED_LIBS:BOOL        | shared library         | ON      |
|           | ENABLE_TEST:BOOL              | testing                | OFF     |
| __OSX__   | USE_APPLICATION_SERVICES:BOOL | framework              | ON      |
|           | USE_IOKIT:BOOL                | framework              | ON      |
|           | USE_OBJC:BOOL                 | obj-c api              | ON      |
|           | USE_CARBON_LEGACY:BOOL        | legacy framework       | OFF     |
| __Win32__ |                               |                        |         |

## Usage
* [Hook Demo](demo/demo_hook.c)
* [Async Hook Demo](demo/demo_hook_async.c)
* [Event Post Demo](demo/demo_post.c)
* [Properties Demo](demo/demo_properties.c)
* [Public Interface](include/uiohook.h)
* Please see the man pages for function documentation.
