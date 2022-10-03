# OoTMM

A very (very) experimental Ocarina of Time/Majora's Mask combo randomizer.

## Setup

You need the following roms:

 * Ocarina of Time (U or J, 1.0)
 * Majora's Mask (U only)

The roms need to be in their native byte-order (big endian). You can use programs such as Tool 64 to convert the roms to the correct format.
The roms need to be named `oot.z64` and `mm.z64` respectively and placed in the `roms` folder.

## Build

You can build OoTMM with either docker or your native environment.

### Docker (recommended)

Install docker, then run:

    docker compose up --build

The first build can be a bit slow. Subsequent builds will be much faster.
On older versions of docker, you may need to run `docker-compose up --build` instead (note the dash).

The output will be `dist/OoTMM.z64`.

### Native (for developers)

You need:

 * A C/C++ toolchain for your system
 * Make
 * [n64-ultra](https://github.com/glankk/n64)
 * Node 18 or newer
 * Git

On windows, we recommend using [WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10) to build OoTMM.

Install the required dependencies:

    npm i
    ./install-deps.sh

Then you can build the project:

    npm start

The output will be `dist/OoTMM.z64`.

## License

This project is licensed under the [MIT license](LICENSE).

## Author

This project was created by [Maxime Bacoux "Nax"](https://github.com/Nax).
