# Dungeon Siege OpenGL Model Viewer

A legacy OpenGL-based model viewer for inspecting Dungeon Siege ASP models,
RAW textures, and PRS animations.

This project is being preserved and cleaned up as a reference implementation
for Dungeon Siege model loading, animation playback, skeleton visualization,
and file-format research.

Dungeon Siege game assets are not included.

## Features

- Loads Dungeon Siege ASP model files
- Loads Dungeon Siege RAW texture files
- Loads and plays Dungeon Siege PRS animation files
- Resolves short asset names through a local NamingKey file
- Supports properties-based local configuration
- Orbit camera controls for inspecting animated models
- Optional wireframe rendering
- Optional animated skeleton debug rendering
- Debug logging toggle
- MSVC debug memory leak detection during clean shutdown

## Third-party dependencies

This project uses freeglut for window/context/input handling.

The freeglut headers, import library, and DLL are included under
`external/freeglut/` for convenience. freeglut is distributed under an
MIT/X11-style license. See `external/freeglut/COPYING`.

## Dungeon Siege assets

Dungeon Siege game assets are not included in this repository.

To run the viewer, create a local properties file based on:

```text
config/viewer.example.properties
```

Copy it to:

```text
config/viewer.properties
```

Then update the paths to point to your local Dungeon Siege data/art folders
and NamingKey file.

Example:

```properties
dungeonSiege.dataRoot=D:/data/dungeon-siege
dungeonSiege.artRoot=D:/data/dungeon-siege/art
dungeonSiege.namingKeyPath=D:/data/dungeon-siege/art/namingkey101.nnk

resources.modelBasePath=D:/data/dungeon-siege/art/meshes
resources.animationBasePath=D:/data/dungeon-siege/art/animations
resources.textureBasePath=D:/data/dungeon-siege/art/bitmaps

viewer.model=m_c_ebb_dg_pos_1
viewer.animation=a_c_ebb_dg_fs0_at

viewer.debugLogging=false
viewer.startWireframe=false
viewer.startPaused=false
viewer.startRotating=false
```

`config/viewer.properties` is local machine configuration and should not be
committed.

## Controls

| Control | Action |
|---|---|
| Left mouse drag | Orbit around the model |
| Right mouse drag | Pan camera |
| Mouse wheel | Zoom in/out |
| C | Reset camera and model rotation |
| R | Toggle automatic model rotation |
| B | Toggle skeleton/bone debug rendering |
| M | Toggle wireframe rendering |
| P | Pause/resume animation |
| L | Toggle verbose debug logging |
| D | Dump bone information |
| Esc | Exit cleanly |

## Build notes

This project currently targets Windows and Visual Studio.

The project uses freeglut from:

```text
external/freeglut/
```

The Visual Studio project should copy `freeglut.dll` to the output directory as
a post-build step.

## Current status

The viewer currently loads and animates complex Dungeon Siege models such as
the dragon and farmboy. It is useful as a known-good OpenGL reference for
future work on the Direct3D 9 viewer animation path.

## Repository policy

This repository should contain source code, project files, documentation,
configuration templates, and third-party dependency files only.

Do not commit Dungeon Siege game assets, extracted asset folders, local
configuration files, or NamingKey data files.

## Legal

This project does not include Dungeon Siege game assets. To use the viewer,
you must provide your own local copy of the game data.

Dungeon Siege is owned by its respective rights holders. This project is an
unofficial fan/research tool and is not affiliated with or endorsed by the
owners of Dungeon Siege.
