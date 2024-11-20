<p align="center">
  <img alt="OoTMM Logo" with="256" height="256" src="https://raw.githubusercontent.com/OoTMM/OoTMM/master/packages/gui/app/assets/logo.png"/>
  <p align="center">The Ocarina of Time/Majora's Mask Combo Randomizer</p>
</p>

![badge-version](https://img.shields.io/github/v/tag/OoTMM/OoTMM?label=version&sort=semver)
![badge-license](https://img.shields.io/github/license/OoTMM/OoTMM)
![badge-build](https://img.shields.io/github/actions/workflow/status/OoTMM/OoTMM/ci.yml?branch=develop)
![badge-stars](https://img.shields.io/github/stars/OoTMM/OoTMM)
![badge-discord](https://discordapp.com/api/guilds/1004394204992118935/widget.png?style=shield)

## Overview

OoTMM is a randomizer for the N64 games Ocarina of Time and Majora's Mask.
It combines both games into a single rom and randomizes the placement of most items in the games.
A large number of settings are available to customize the experience.

## What's new?

We maintain a comprehensive [changelog](CHANGELOG.md) that details all the changes in each release.

## Usage

We **strongly** recommend that you use the [Web-based generator](https://ootmm.com).

## Support

If you need help, you can ask for help on our [Discord server](https://discord.gg/4QdtPBP6wf).

## Manual Setup

You need the following roms:

 * Ocarina of Time (U or J, 1.0)
 * Majora's Mask (U only)

The roms need to be in their native byte-order (big endian). You can use programs such as Tool 64 to convert the roms to the correct format.
The roms need to be named `oot.z64` and `mm.z64` respectively and placed in the `roms` folder.

## Build

You can build OoTMM with either a DevContainer or your native environment.

### DevContainer (recommended)

You need:

 * Docker
 * Visual Studio Code
 * Remote - Containers extension

Open the project in Visual Studio Code and click on the "Reopen in Container" button that appears in the bottom right corner.
That's it.

### Native

You need:

 * A C/C++ toolchain for your system
 * CMake
 * Ninja
 * [n64-ultra](https://github.com/glankk/n64)
 * Node 22 or newer
 * Git

On windows, we recommend using [WSL2](https://learn.microsoft.com/en-us/windows/wsl/install) to build OoTMM.

Install everything you need:

    ./setup.sh

### Using the build

You can generate a seed:

    pnpm start:core:config

The output will be `out/OoTMM.z64`.

## License

This project is licensed under the [MIT license](LICENSE).

## Author

This project was created by [Maxime Bacoux "Nax"](https://github.com/Nax).
