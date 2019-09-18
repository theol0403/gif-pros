#pragma once
#include "main.h"

#include "gifdec.h"

class Gif {

public:

  Gif( const char *fname, lv_obj_t* parent, int sx, int sy );
  ~Gif();

private:
  gd_GIF *_gif;
  int    _sx;
  int    _sy;
  void   *_gifmem;
  uint8_t   *_buffer;

  lv_color_t *_cbuf;
  lv_obj_t *_canvas;

  pros::task_t _task;

  void _cleanup();
  void _render();

  static void _render_task(void *arg);

};