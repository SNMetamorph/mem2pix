# mem2pix
Program for visualizing arbitrary part of remote process memory in real-time.<br> May be useful for working with things like
framebuffers/textures and tracking how these stuff content changes over the time.
* Supports many pixel format
* Works on both Linux and Windows (including XP)
* Based on SDL2 library
## Images
![Title Text](https://i.imgur.com/L3mZPy0.png)
![Picture 1](https://i.imgur.com/6tLsdDd.png)
![Picture 2](https://i.imgur.com/t6Yd5qm.png)
![Picture 3](https://i.imgur.com/2k2ou6K.png)

# Building 
## Windows
1) Clone this repository.
```
git clone --recursive https://github.com/SNMetamorph/mem2pix.git
cd mem2pix
git submodule update --init --recursive
```
2) Open CMakeLists.txt using Visual Studio
3) Select desired build type
4) Build entire project
## Linux
1) Make sure that you has all required packages (cmake, gcc, etc.)
2) Clone this repository, as described above.
3) Run these commands
```
mkdir -p build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
make
```
Result executable file will be located in /build/bin/*BUILD_TYPE* directory.

# About
This project uses [SDL2](https://www.libsdl.org/index.php) library, which has [zlib](https://www.libsdl.org/license.php) license.
