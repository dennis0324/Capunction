hookfunctionkey: keyboard for remote computer in WINDOW
====================================================================

![build-binaires](https://github.com/kwhat/libuiohook/workflows/build-binaires/badge.svg)

## ABOUT

this is for people code with remote computer(eg. moonlight streaming, goole remote desktop).

using keyboard that doesn't have function keys(eg. f1,f2, etc.)

this make you to use caplock to fn key and send fucntion keys

## Key binds



| key binds         | output                        | description            | 
| ---------         | ----------------------------- | ---------------------- | 
| `caplock` + `1`   | `F1`                          |                        | 
| `caplock` + `2`   | `F2`                          |                        | 
| `caplock` + `3`   | `F3`                          |                        | 
| `caplock` + `4`   | `F4`                          |                        | 
| `caplock` + `5`   | `F5`                          |                        | 
| `caplock` + `6`   | `F6`                          |                        | 
| `caplock` + `7`   | `F7`                          |                        | 
| `caplock` + `8`   | `F8`                          |                        | 
| `caplock` + `9`   | `F9`                          |                        | 
| `caplock` + `0`   | `F10`                         |                        | 
| `caplock` + `-`   | `F11`                         |                        | 
| `caplock` + `=`   | `F12`                         |                        | 
| `caplock` + `backspace`   | `Delete`              | `shift` + `backspace` wouldn't work on my case so i made it | 
| `caplock` + `backslash`   | `Insert`              |                        | 
| `caplock` + `←`   | `Home`                        |                        | 
| `caplock` + `→`   | `End`                         |                        | 
| `caplock` + `↑`   | `Pgup`                        |                        | 
| `caplock` + `↓`   | `Pgdn`                        |                        | 
| `caplock` + `↓`   | `Pgdn`                        |                        | 


## used library

[libuiohook](https://github.com/kwhat/libuiohook) - made by [kwhat](https://github.com/kwhat)

[tray](https://github.com/zserge/tray) - made by [zserge](https://github.com/zserge/)

## Compiling
Prerequisites: 
 * [cmake](https://cmake.org)
 * gcc, clang or msvc

```
$ git clone https://github.com/kwhat/libuiohook
$ cd libuiohook
$ mkdir build && cd build
$ cmake -S .. -DCMAKE_INSTALL_PREFIX=../dist
$ cmake --build . --parallel 2 --target install  
```
