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