#ifndef WINDOW_WINDOW_H_
#define WINDOW_WINDOW_H_

class GLFWwindow;

class Window {
public:
	static GLFWwindow* window;
	static int width;
	static int height;

	static int  initialize(int width, int height, const char* title);
	static void terminate();

	static bool shouldClose();
	static void setShouldClose(bool flag);
	static void setCursorMode(int mode);
	static void swapBuffers();
};

#endif