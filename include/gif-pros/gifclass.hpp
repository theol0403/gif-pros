#pragma once
#include "main.h"

#include "gifdec.h"

class Gif {

public:

  Gif( const char *fname, lv_obj_t* parent);
  ~Gif();

private:
  gd_GIF *_gif = nullptr;
  void   *_gifmem = nullptr;
  uint8_t   *_buffer = nullptr;

  lv_color_t *_cbuf = nullptr;
  lv_obj_t *_canvas = nullptr;

  pros::task_t _task;

  void _cleanup();
  void _render();

  static void _render_task(void *arg);

};