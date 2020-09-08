#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
bool mask[WINDOW_HEIGHT][WINDOW_WIDTH] = { false };
GLFWwindow *window;

struct color { float r, g, b; };

struct colorset {
	struct color white {255,255,255 };
	struct color red { 255,0,0 };
	struct color blue { 0,0,255 };
	struct color green { 0,255,0 };
	struct color magenta {255,0,255};
	struct color black {0,0,0};
	struct color cyan {0,255,255};
	struct color yellow {255,255,0};
	struct color background = black;
};
struct brushes {
	double size = 1;
	char shape = 's';
	bool spray = false;
	struct color colorpick = { 255,255,255 };
};
struct colorset colorz;
struct brushes brush;

// Color structure. Can be used to define the brush and background color.

// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))

// Set a particular pixel of the frameBuffer to the provided color
void SetFrameBufferPixel(int x, int y, struct color lc)
{
	// Origin is upper left corner. 
	// Changes the origin from the lower-left corner to the upper-left corner
	y = WINDOW_HEIGHT - 1 - y;

	x = CLAMP(x, 0, WINDOW_WIDTH - 1);
	y = CLAMP(y, 0, WINDOW_HEIGHT - 1);

	frameBuffer[y][x][0] = lc.r;
	frameBuffer[y][x][1] = lc.g;
	frameBuffer[y][x][2] = lc.b;

}

void ClearFrameBuffer()
{
	memset(frameBuffer, 0.0f, sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
}

// Display frameBuffer on screen
void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

// Mouse position callback function
void CursorPositionCallback(GLFWwindow* lWindow, double xpos, double ypos)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		std::cout << "Mouse position is: x - " << xpos << ", y - " << ypos << std::endl;
		int spraynum;
		if (brush.shape == 's') {
			for (int i = xpos - brush.size; i < xpos + brush.size; i++) {
				for (int j = ypos - brush.size; j < ypos + brush.size; j++) {
					if (brush.spray == true) {
						spraynum = rand() % 200;
						if (spraynum <=2) {
							mask[j][i] = true;
							SetFrameBufferPixel(i, j, brush.colorpick);
						}
					}
					else {
						mask[j][i] = true;
						SetFrameBufferPixel(i, j, brush.colorpick);
					}
				}
			}
		}

		else if (brush.shape == 'c') {
			for (int i = xpos - brush.size; i < xpos + brush.size; i++) {
				for (int j = ypos - brush.size; j < ypos + brush.size; j++) {
					if ((i-xpos) * (i-xpos) + (j-ypos) * (j-ypos) <= brush.size*brush.size) {
						if (brush.spray == true) {
							spraynum = rand() % 200;
							if (spraynum <= 2) {
								mask[j][i] = true;
								SetFrameBufferPixel(i, j, brush.colorpick);
							}
						}
						else {
							mask[j][i] = true;
							SetFrameBufferPixel(i, j, brush.colorpick);
						}
					}
				}
			}
		}
	}
}

// Mouse callback function
void MouseCallback(GLFWwindow* lWindow, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
	{
		std::cout << "Mouse left button is pressed." << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		for (int i = 0; i < WINDOW_WIDTH; i++) {
			for (int j = 0; j < WINDOW_HEIGHT; j++) {
				SetFrameBufferPixel(i, j, colorz.background);
				mask[j][i] = false;
			}
		}
	}

}

// You can use "switch" or "if" to compare key to a specific character.
// for example,
// case '0')
//     DO SOMETHING

// Keyboard callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	std::cout << "Key " << (char)key << " is pressed." << std::endl;
	switch (key) {
		case 's' : 
			if (brush.spray == true) {
				brush.spray = false;
			}
			else {
				brush.spray = true;
			}
			break;
		case ')' :
			for (int i = 0; i < WINDOW_WIDTH; i++) {
				for (int j = 0; j < WINDOW_HEIGHT; j++) {
					if (mask[j][i] != true) {
						SetFrameBufferPixel(i, j, colorz.black);
						colorz.background = colorz.black;
					}
				}
			}
			break;
		case '!' :
			for (int i = 0; i < WINDOW_WIDTH; i++) {
				for (int j = 0; j < WINDOW_HEIGHT; j++) {
					if (mask[j][i] != true) {
						SetFrameBufferPixel(i, j, colorz.blue);
						colorz.background = colorz.blue;
					}
				}
			}
			break;
		case '@' :
			for (int i = 0; i < WINDOW_WIDTH; i++) {
				for (int j = 0; j < WINDOW_HEIGHT; j++) {
					if (mask[j][i] != true) {
						SetFrameBufferPixel(i, j, colorz.green);
						colorz.background = colorz.green;
					}
				}
			}
			break;
		case '#' :
			for (int i = 0; i < WINDOW_WIDTH; i++) {
				for (int j = 0; j < WINDOW_HEIGHT; j++) {
					if (mask[j][i] != true) {
						SetFrameBufferPixel(i, j, colorz.cyan);
						colorz.background = colorz.cyan;
					}
				}
			}
			break;
		case '$' : 
			for (int i = 0; i < WINDOW_WIDTH; i++) {
				for (int j = 0; j < WINDOW_HEIGHT; j++) {
					if (mask[j][i] != true) {
						SetFrameBufferPixel(i, j, colorz.red);
						colorz.background = colorz.red;
					}
				}
			}
			break;
		case '%' :
			for (int i = 0; i < WINDOW_WIDTH; i++) {
				for (int j = 0; j < WINDOW_HEIGHT; j++) {
					if (mask[j][i] != true) {
						SetFrameBufferPixel(i, j, colorz.magenta);
						colorz.background = colorz.magenta;
					}
				}
			}
			break;
		case '^' :
			for (int i = 0; i < WINDOW_WIDTH; i++) {
				for (int j = 0; j < WINDOW_HEIGHT; j++) {
					if (mask[j][i] != true) {
						SetFrameBufferPixel(i, j, colorz.yellow);
						colorz.background = colorz.yellow;
					}
				}
			}
			break;
		case '&' :
			for (int i = 0; i < WINDOW_WIDTH; i++) {
				for (int j = 0; j < WINDOW_HEIGHT; j++) {
					if (mask[j][i] != true) {
						SetFrameBufferPixel(i, j, colorz.white);
						colorz.background = colorz.white;
					}
				}
			}
			break;
		case '0' :
			brush.colorpick = colorz.black;
			break;
		case '1' :
			brush.colorpick = colorz.blue;
			break;
		case '2' :
			brush.colorpick = colorz.green;
			break;
		case '3' :
			brush.colorpick = colorz.cyan;
			break;
		case '4' :
			brush.colorpick = colorz.red;
			break;
		case '5' :
			brush.colorpick = colorz.magenta;
			break;
		case '6' :
			brush.colorpick = colorz.yellow;
			break;
		case '7' :
			brush.colorpick = colorz.white;
			break;
		case 'b' :
			if (brush.shape == 's') {
				brush.shape = 'c';
			}
			else {
				brush.shape = 's';
			}
			break;
		case '+' :
			if (brush.size < 128) {
				brush.size *= 2;
			}
			break;
		case '-' :
			if (brush.size > 1) {
				brush.size /= 2;
			}
			break;
	}
}

void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment 1 - <Dominic Rivas>", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetCharCallback(window, CharacterCallback);
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();
}


int main()
{	
	Init();
	while (glfwWindowShouldClose(window) == 0)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}