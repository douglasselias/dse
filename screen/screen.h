typedef float f32;

typedef struct
{
  f32 min_x, min_y, max_x, max_y;
} Rect;

Rect calculate_optimal_size_for_render_size(f32 window_width, f32 window_height, f32 render_width, f32 render_height)
{
  Rect r = {};

  u32 optimal_width  = window_height * (render_width  / render_height);
  u32 optimal_height = window_width  * (render_height / render_width);

  if(optimal_width > window_width)
  {
    // Width constrained
    r.min_x = 0;
    r.max_x = window_width;

    f32 half_empty_space = (window_height - optimal_height) / 2;
    r.min_y = half_empty_space;
    r.max_y = r.min_y + optimal_height;
  }
  else
  {
    // Height constrained
    r.min_y = 0;
    r.max_y = window_height;

    f32 half_empty_space = (window_width - optimal_width) / 2;
    r.min_x = half_empty_space;
    r.max_x = r.min_x + optimal_width;
  }

  return r;
}

typedef struct {
  f32 mouse_x;
  f32 mouse_y;
  bool mouse_down;
  s32 hot_item;
  s32 active_item;
} UIState;

UIState ui_state = {0};
SDL_FRect button_rect = {0};
SDL_FRect slider_rect = {0};
SDL_FRect arrow_rect = {0};

bool button(s32 id, Rect rect)
{
  Vector2 mouse = {ui_state.mouse_x, ui_state.mouse_y};

  if(SDL_PointInRectFloat(&mouse, &rect))
  {
    ui_state.hot_item = id;
    if(ui_state.active_item == false && ui_state.mouse_down)
    {
      ui_state.active_item = id;
    }
  }

  if(ui_state.hot_item == id)
  {
    if(ui_state.active_item == id)
    {
      SDL_SetTextureColorMod(button_pressed, 18, 114, 154);
      SDL_RenderTexture(renderer, button_pressed, NULL, &rect);
    }
    else
    {
      SDL_SetTextureColorMod(button_texture, 18, 114, 154);
      SDL_RenderTexture(renderer, button_texture, NULL, &rect);
    }
  }
  else
  {
    SDL_SetTextureColorMod(button_texture, 255, 255, 255);
    SDL_RenderTexture(renderer, button_texture, NULL, &rect);
  }

  if(ui_state.mouse_down == false
  && ui_state.hot_item == id
  && ui_state.active_item == id)
  {
    return true;
  }

  return false;
}