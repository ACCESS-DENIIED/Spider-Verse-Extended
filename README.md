# Spider-Verse Extended

A DLL proxy injector specifically designed for Marvel's Spider-Man Remastered and Marvel's Spider-Man: Miles Morales PC games. This tool enables dynamic mod loading by implementing a proxy for AMD's AGS library.

## Overview

Spider-Verse Extended works by masquerading as `amd_ags_x64.dll`, implementing the AMD Gaming Services (AGS) interface while providing additional functionality for mod injection. This approach ensures compatibility and stability with the base game while enabling mod support.

## Features

- Seamless DLL proxy implementation of AMD AGS
- Automatic mod detection and injection
- Dynamic loading of multiple mods from a dedicated `scripts` folder
- Support for both Marvel's Spider-Man Remastered and Miles Morales PC
- Lightweight and efficient implementation
- No modification of original game files

## Installation

1. Download the latest release from the Releases page
2. Extract `amd_ags_x64.dll` and `amd_ags_x64_orig.dll` to your game's installation directory
3. Create a `scripts` folder in the same directory
4. Place your mod DLLs in the `scripts` folder
5. Launch the game normally

## Technical Details

The injector works by:
1. Implementing the complete AMD AGS SDK interface
2. Forwarding all AGS calls to the original DLL
3. Scanning and loading mod DLLs from the `scripts` folder during initialization
4. Managing mod injection without interfering with the game's core functionality

## Requirements

- Windows OS
- Marvel's Spider-Man Remastered PC or Marvel's Spider-Man: Miles Morales PC
- Visual C++ Redistributable (latest version recommended)

## Building from Source

The project can be compiled using:
```bash
g++ -shared -o amd_ags_x64.dll Spider-Verse-Extended.cpp -lstdc++

## Uninstall

1. Delete `amd_ags_x64.dll`
2. Rename `amd_ags_x64_orig.dll` back to `amd_ags_x64.dll`
