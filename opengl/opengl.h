#include <windows.h>
#include <GL/gl.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

typedef int   s32;
typedef float f32;

typedef struct
{
  f32 r, g, b, a;
} GL_Color;

typedef struct
{
  f32 x, y, z;
} GL_Vertex;

HWND window;
PAINTSTRUCT ps;
HDC hdc;

void init_opengl(HWND hwnd)
{
  window = hwnd;
  HDC device_context = GetDC(window);

  PIXELFORMATDESCRIPTOR pixel_format_descriptor = {0};
  pixel_format_descriptor.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
  pixel_format_descriptor.nVersion   = 1;
  pixel_format_descriptor.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pixel_format_descriptor.iPixelType = PFD_TYPE_RGBA;
  pixel_format_descriptor.cColorBits = 32;
  pixel_format_descriptor.cDepthBits = 16;
  pixel_format_descriptor.iLayerType = PFD_MAIN_PLANE;

  s32 pixel_format = ChoosePixelFormat(device_context, &pixel_format_descriptor);
  SetPixelFormat(device_context, pixel_format, &pixel_format_descriptor);

  HGLRC hglrc = wglCreateContext(device_context);
  wglMakeCurrent(device_context, hglrc);
}

void begin_drawing(GL_Color color)
{
  hdc = BeginPaint(window, &ps);

  glClearColor(color.r, color.g, color.b, color.a);

  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_TRIANGLES);
}

void end_drawing()
{
  glEnd();
  glFlush();

  SwapBuffers(hdc);
  EndPaint(window, &ps);
}

void draw_vertex(GL_Color color, GL_Vertex vertex)
{
  glColor4f(color.r, color.g, color.b, color.a);
  glVertex3f(vertex.x, vertex.y, vertex.z);
}