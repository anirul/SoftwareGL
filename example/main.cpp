#include <iostream>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
	#define WINDOWS_LEAN_AND_MEAN
	#include <windows.h>
#endif

#include <GL/glew.h>
#include <SDL.h>

#if defined(_WIN32) || defined(_WIN64)
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")
	#pragma comment(lib, "SDL2.lib")
	#pragma comment(lib, "glew32.lib")
#endif

#include "WindowSDL2GL.h"
#include "WindowSoftwareGL.h"

#if defined(_WIN32) || defined(_WIN64)
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
#else
int main(int ac, char** av)
#endif
{
	WindowSDL2GL window(std::make_shared<WindowSoftwareGL>());
	window.Startup();
	return 0;
}