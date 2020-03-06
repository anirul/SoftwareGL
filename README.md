# Software GL

This is a project to try to make a Software GL implementation, and then see the
OpenGL version of it. The files loaded to render the project are found under the
`asset` folder.

## Working

You can load files as `.obj` and image as `.tga` files, the software or the 
OpenGL implementation will load them and show them in a window on the screen the
FPS could be lousy in the software version as the goal was more an educative one
than a real performance oriented version of it.

## Software GL

![SoftwareGL](https://github.com/anirul/SoftwareGL/raw/master/image/torus.png "A textured torus rendered by Software.")

## OpenGL (4.x)

![OpenGL](https://github.com/anirul/SoftwareGL/raw/master/image/torus_opengl.png "A textured torus rendered by OpenGL.")

## Interface Buttons

- `ESC` to quit.

## CMake help

It use [VCPKG](https://github.com/microsoft/vcpkg) to manage project and 
settings, and CMake to make the project files. You can call 
[CMake](https://cmake.org/) (after installing it) with the following line under
windows:

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows
```

## Todo list

The software version still has some issue with texture they are not rendered
correctly (see [affine](https://en.wikipedia.org/wiki/Texture_mapping) transform
problem). There is also some problems with the way triangle are  interpolated
and the solution should be to implemente a better algorithm. This is started but
is not working yet, it should also improve performances.

On the OpenGL version it could be cool to have PBR also implemented and then to
backport it to the Software version.
