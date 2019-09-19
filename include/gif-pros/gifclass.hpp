#pragma once
#include "main.h"
#include "gifdec.h"

/**
 * MIT License
 * Copyright (c) 2019 Theo Lemay
 * https://github.com/theol0403/gif-pros
 */

class Gif {

public:

  /**
   * Used to specify which transparency mode is used.
   * There are three transparency states:
   *   - automatic: uses a flag in the gif file to deduce if the gif is transparent.
   *   - force: uses transparancy regardless of the flag.
   *   - off: does not use transparency and instead uses gif background color.
   * There are two tranparency modes:
   *   - dynamic: uses a 0-255 transparency value for each pixel. May not work well with some images.
   *   - boolean: uses either a 0 or 255 transparency value for each pixel. Used to only make the background transparent.
   * There are four available transparency settings:
   */
  enum class Transparency {
    automatic, // (default) uses dynamic transparency mode only if transparency flag is set in the gif file
    dynamic,   // (force) enables dynamic transparency mode where transparency is range of 0-100%
    boolean,   // (force) enables boolean transparency mode where transparency is either 0% or 100%
    off        // disables transparency and uses gif background color, sometimes white
  };

  /**
   * Construct the Gif class
   * @param fname  the gif filename on the SD card (prefixed with /usd/)
   * @param parent the LVGL parent object
   * @param mode   the transparency mode
   */
  Gif(const char* fname, lv_obj_t* parent, Transparency mode = Gif::Transparency::automatic);

  /**
   * Destructs and cleans the Gif class
   */
  ~Gif();

private:

  gd_GIF* _gif = nullptr; // gif decoder object
  void* _gifmem = nullptr; // gif file loaded from SD into memory 
  uint8_t* _buffer = nullptr; // decoder frame buffer

  lv_color_t* _cbuf = nullptr; // canvas buffer
  lv_obj_t* _canvas = nullptr; // canvas object

  pros::task_t _task = nullptr; // render task

  Transparency _mode = Gif::Transparency::automatic; // transparency mode

  /**
   * Cleans and frees all allocated memory
   */
  void _cleanup();

  /**
   * Render cycle, blocks until loop count exceeds gif loop count flag (if any)
   */
  void _render();

  /**
   * Calls _render()
   * @param arg Gif*
   */
  static void _render_task(void* arg);

};