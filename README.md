# mem2pix
![GitHub release (by tag)](https://img.shields.io/github/downloads/SNMetamorph/mem2pix/total)
![GitHub top language](https://img.shields.io/github/languages/top/SNMetamorph/mem2pix)
![GitHub repo size](https://img.shields.io/github/repo-size/SNMetamorph/mem2pix)
![GitHub commit activity](https://img.shields.io/github/commit-activity/m/SNMetamorph/mem2pix)
![Lines of code](https://img.shields.io/tokei/lines/github/SNMetamorph/mem2pix)
![GitHub Release Date](https://img.shields.io/github/release-date/SNMetamorph/mem2pix)

Program for visualizing arbitrary part of remote process memory in real-time.  
May be useful for working with things like
framebuffers/textures and tracking how these stuff content changes over the time.
* Supports many pixel formats
* Works on both Linux and Windows (including XP)
* Based on SDL2 library
## Images
![Title Text](https://i.imgur.com/RAV52cz.png)
![Picture 1](https://i.imgur.com/6tLsdDd.png)
![Picture 2](https://i.imgur.com/t6Yd5qm.png)

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
Result executable file will be located in /build/*BUILD_TYPE*/bin directory.

# External depedencies
[SDL2](https://www.libsdl.org/index.php) - Copyright (c) Sam Lantinga - [zlib](https://www.libsdl.org/license.php) license
