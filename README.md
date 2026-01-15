# 3D Rasterizer in C (SDL2)

This project is a small software 3D rendering engine written entirely in C, using SDL2 only for window creation and input handling. All rendering logic is implemented from scratch, without relying on graphics APIs such as OpenGL or Vulkan.

The renderer implements the full basic 3D rasterization pipeline and is capable of rendering complex textured meshes with per-pixel lighting.

## Features

- Software rasterization (CPU-based)
- Custom math library (vectors, matrices, transformations)
- Perspective projection
- Triangle rasterization
- Depth buffering (Z-buffer)
- Texture mapping
- Per-pixel lighting

## Technical Overview

The engine follows a traditional rasterization pipeline:
1. Model, view, and projection transformations
2. Triangle clipping and screen-space projection
3. Per-pixel triangle rasterization
4. Depth testing
5. Texture sampling and lighting computation

All components — including math, rasterization, texturing, and lighting — are implemented manually in C.

## Dependencies

- SDL2 (used only for window creation, framebuffer display, and input)
- stb_image.h (used for loading images for textures)

## Purpose

This project is intended as a low-level exploration of 3D rendering. It is not a full-featured engine, but it demonstrates the core techniques required to build a functional software renderer capable of displaying complex 3D objects.

## Build & Run

To build and run the project just run the run.bat file.
