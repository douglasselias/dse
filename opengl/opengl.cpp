#include "opengl.h"

struct Vertex
{
  GL_Color color;
  GL_Vertex position;
};

Vertex vertices[3] =
{
  { {1.0f, 0.0f, 0.0f}, { 0.0f,  0.5f, 0.0f} },
  { {0.0f, 1.0f, 0.0f}, {-0.5f, -0.5f, 0.0f} },
  { {0.0f, 0.0f, 1.0f}, { 0.5f, -0.5f, 0.0f} },
};

GL_Color background_color = {27.0f / 255.0f, 24.0f / 255.0f, 26.0f / 255.0f, 1.0f};

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  {
    case WM_PAINT:
    {
      // PAINTSTRUCT ps;
      // HDC hdc = BeginPaint(hwnd, &ps);

      begin_drawing(background_color);
        for(s32 i = 0; i < 3; i++)
        {
          Vertex v = vertices[i];
          draw_vertex(v.color, v.position);
        }
      end_drawing();

      // SwapBuffers(hdc);
      // EndPaint(hwnd, &ps);
      return 0;
    }
    case WM_DESTROY:
    {
      PostQuitMessage(0);
      return 0;
    }
  }

  return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
  WNDCLASS wc = {0};
  wc.lpfnWndProc = WndProc;
  wc.lpszClassName = "OpenGLExample";
  RegisterClass(&wc);

  s32 screen_width  = GetSystemMetrics(SM_CXSCREEN);
  s32 screen_height = GetSystemMetrics(SM_CYSCREEN);
  s32 window_width  = 1080;
  s32 window_height = 720;
  s32 window_x = (screen_width  / 2) - (window_width  / 2);
  s32 window_y = (screen_height / 2) - (window_height / 2);

  HWND hwnd = CreateWindow("OpenGLExample", "Colored Triangle", WS_OVERLAPPEDWINDOW, window_x, window_y, window_width, window_height, NULL, NULL, hInstance, NULL);

  init_opengl(hwnd);
  ShowWindow(hwnd, nCmdShow);

  MSG msg;
  while(GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}