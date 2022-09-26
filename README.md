
<p align="center">
  <img src = "https://user-images.githubusercontent.com/50710829/192276651-99b2c137-3ed1-428f-8716-8c2eab20e961.png" width="300px" >
</p>
<h3 align="center">Capunction</h3>
<p align="center" style="font-size:30px !important;">
key binding for remote desktop in WINDOW due to non function keyboard
</p>

![build-binaires](https://github.com/kwhat/libuiohook/workflows/build-binaires/badge.svg)

## ABOUT
<h3>what is Capunction?</h3>
It is cap + function!<br>

This is for people that need a function key when using remote desktop software(eg. moonlight streaming, goole remote desktop).

using keyboard that doesn't have function keys(eg. f1,f2, etc.)

this make you to use caplock to fn key and send fucntion keys


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
| `caplock` +  `caplock`   |  `caplock`                        |                        | 


## used library

[libuiohook](https://github.com/kwhat/libuiohook) - made by [kwhat](https://github.com/kwhat)

[tray](https://github.com/zserge/tray) - made by [zserge](https://github.com/zserge/)


## LICENSE


