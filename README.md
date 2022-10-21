
<p align="center">
  <img src = "https://user-images.githubusercontent.com/50710829/192276651-99b2c137-3ed1-428f-8716-8c2eab20e961.png" width="300px" >
</p>
<h3 align="center">Capunction</h3>
<p align="center" style="font-size:30px !important;">
key binding for remote desktop in WINDOW due to non function keyboard
</p>


## ABOUT
<h3>what is Capunction?</h3>
It is cap + function!<br>

<h3>Recommended to</h3>

 * This is for people that need a function key when using remote desktop software(eg. moonlight streaming, google remote desktop).
 * if your function keys doesn't work properly.
 * Using keyboard that doesn't have function keys(eg. f1,f2, etc.)
 
 this make you to use capslock to fn key 


## Compiling
Prerequisites: 
 * [cmake](https://cmake.org)
 * gcc, clang or msvc

```
$ git clone https://github.com/dennis0324/Capunction.git
$ cd Capunction
$ mkdir build && cd build
$ cmake -S .. -DCMAKE_INSTALL_PREFIX=../dist // this is for building and testing
$ cmake -S .. -DCMAKE_INSTALL_PREFIX=../dist -DPUBLISH=TRUE // this is for publish for use
$ cmake --build . --parallel 2 --target install  
```



## Key binds



| key binds         | output                        | description            | 
| ---------         | ----------------------------- | ---------------------- | 
| `capslock` + `1`   | `F1`                          |                        | 
| `capslock` + `2`   | `F2`                          |                        | 
| `capslock` + `3`   | `F3`                          |                        | 
| `capslock` + `4`   | `F4`                          |                        | 
| `capslock` + `5`   | `F5`                          |                        | 
| `capslock` + `6`   | `F6`                          |                        | 
| `capslock` + `7`   | `F7`                          |                        | 
| `capslock` + `8`   | `F8`                          |                        | 
| `capslock` + `9`   | `F9`                          |                        | 
| `capslock` + `0`   | `F10`                         |                        | 
| `capslock` + `-`   | `F11`                         |                        | 
| `capslock` + `=`   | `F12`                         |                        | 
| `capslock` + `backspace`   | `Delete`              | `shift` + `backspace` wouldn't work on my case so i made it | 
| `capslock` + `backslash`   | `Insert`              |                        | 
| `capslock` + `←`   | `Home`                        |                        | 
| `capslock` + `→`   | `End`                         |                        | 
| `capslock` + `↑`   | `Pgup`                        |                        | 
| `capslock` + `↓`   | `Pgdn`                        |                        | 
| `capslock` + `↓`   | `Pgdn`                        |                        | 
| `capslock` +  `capslock`   |  `capslock`                        | instantly press `capslock` twice will active capslock  | 


## used library

[libuiohook](https://github.com/kwhat/libuiohook) - made by [kwhat](https://github.com/kwhat)

[tray](https://github.com/zserge/tray) - made by [zserge](https://github.com/zserge/)


## LICENSE
Distributed under the GNU LGPLv3 License. See [LICENSE](https://github.com/dennis0324/Capunction) for more information.

