#pragma once
#include "main.h"

#include "gifdec.h"

class Gif {

public:

  Gif( const char *fname, int sx, int sy );
  ~Gif();

private:
  gd_GIF *_gif;
  int    _sx;
  int    _sy;
  void   *_gifmem;
  void   *_buffer;

  pros::task_t _task;

  static void render(void *arg);
  static void render_task(void *arg);

};