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

  static void render(void *arg );

};