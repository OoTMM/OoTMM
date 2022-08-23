# OoTMM

A very (very) experimental Ocarina of Time/Majora's Mask combo randomizer.

## Setup

You need:

 * Ruby (ideally 3.0 or newer, older versions may work)
 * A gcc+binutils toolchain targeting `mips-elf`
 * Ocarina of Time (U or J, 1.0)
 * Majora's Mask (U only)
 * An Unix environment (Linux, BSD, macOS, WSL...)

## Build

Copy the original roms in the `roms` folder. They need to be named `oot.z64` and `mm.z64`, and be in their **native, big endian** order.

Run this command once to install dependencies:

    ./install-deps.sh
    bundle config path vendor/bundle
    bundle install

Run this to build the rom:

    bin/combo

The output will be `dist/OoTMM.z64`.
