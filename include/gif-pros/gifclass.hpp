#pragma once
#include "main.h"

#include "gifdec.h"

class Gif {

public:

  enum class Transparency {
    automatic, // uses dynamic transparency mode if transparency is enabled in the gif
    dynamic,   // enables dynamic transparency mode (force transparency) where transparency is range of 0-100%
    boolean,   // enables boolean transparency mode (force transparency) where transparency is either 0% or 100%
    off        // disables transparency and uses gif background color, usually white
  };

  Gif(const char* fname, lv_obj_t* parent, Transparency mode = Gif::Transparency::automatic);
  ~Gif();

private:
  gd_GIF* _gif = nullptr;
  void  * _gifmem = nullptr;
  uint8_t  * _buffer = nullptr;

  lv_color_t* _cbuf = nullptr;
  lv_obj_t* _canvas = nullptr;

  pros::task_t _task;

  Transparency _mode = Gif::Transparency::automatic;

  void _cleanup();
  void _render();

  static void _render_task(void* arg);

};