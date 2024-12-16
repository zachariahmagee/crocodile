#!/bin/bash

# clang -std=c11 ./src/main.c -o bin/build -I/$(pkg-config --libs --cflags raylib)


g++ -std=c++14  ./src/main.cpp -o bin/build -I/$(pkg-config --libs --cflags raylib) \
	-L$(pwd)/lib \
	-lraylib -lEGL -lGLESv2 \
	-Wl,-rpath,@executable_path/../lib

# ~/documents/development/c/crocodile/lib/libEGL.dylib ~/documents/development/c/crocodile/lib/libGLESv2.dylib 
#clang -std=c11 ./src/main.c -o bin/build -I./libs -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib -L./lib -lEGL -lGLESv2
