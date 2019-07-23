# Chip 8 Emulator written in C

Documentation for Chip 8 Emulator

### Prerequisites

- SDL2 Library

### Installing

This project was developed on mac, installing the SDL2 library simply required using `brew install sdl2`
Refer to the SDL2 documentation for installing the library on your preferred platform.

### Makefile

The Makefile is in the root directory of the repository.

The makefile contains a number of useful commands which are documented below:

`make` or `make chip8` will build all object files necessary and put them in the obj directory then proceed
to compile all object files into a single executable, `chip8`.

`make clean` will clean the workspace up removing everything besides the source code & documentation.

### Usage

Running the executable file `chip8` will start the chip 8 emulator.

## Directories

- [src](src/) (Contains source code)
- [include](include/) (Contains header files)

## [main.c](src/main.c)

This code is the main entrypoint for the emulator and glues all components together that characterize the chip 8 emulator.
The SDL2 setup and boilerplate code resides here.

## Authors

- **Erik Haight**

<!-- [flasklink]: http://flask.pocoo.org/docs/1.0/api/ -->
