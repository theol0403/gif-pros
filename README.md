# gif-pros
A GIF decoder written for PROS with LVGL integration and transparency support.

![gif-pros](https://github.com/theol0403/gif-pros/raw/master/gif-pros.png)


Loosely based on jpearman's [VEXcode GIF demo](https://www.vexforum.com/t/animated-gif-demo-vexcode/58755).
Inspired by [this](https://www.vexforum.com/t/gif-in-pros/66187?u=theol0403) VEXForum request.
Implements the [gifdec.c](https://github.com/lecram/gifdec) decoder.

This library supports transparency and implements the LVGL canvas, making it extremely flexible and versatile to use.

# Installation
##### 1a. Pre-built template
Download `gif-pros@<version>.zip`</a> from this repository.
##### 1b. Build
To build the template yourself instead of downloading, clone this repository and run `prosv5 make template`.
##### 2. Fetch template
Run `prosv5 c fetch <template>` to import the template into the CLI.
##### 3. Apply template
Run `prosv5 c apply gif-pros` in your destination project to install the package.

### Standalone project
You can also use this repository as a standalone project and run it directly on the robot.
There is an example program in `src/opcontrol.cpp`.

# Use
After you have installed gif-pros into your project, include the header:
```cpp
#include "gif-pros/gifclass.h"
```
To initialize the GIF reader, run this command:
```cpp
/**
 * Construct the Gif class
 * @param fname  the gif filename on the SD card (prefixed with /usd/)
 * @param parent the LVGL parent object
 * @param mode   the transparency mode
 */
Gif gif("/usd/mygif.gif", lv_scr_act(), Gif::Transparency::automatic);
```

**Note:** The `Gif` object cannot go out of scope or be destroyed. If you need the object to go out of scope, you can use `static` or dynamically create the object using `new`:
```cpp
// this block can go out of scope
static Gif gif("/usd/mygif.gif", lv_scr_act(), Gif::Transparency::automatic);
// or
Gif* gif = new Gif("/usd/mygif.gif", lv_scr_act(), Gif::Transparency::automatic); 
```

### Transparency
The biggest feature of gif-pros is that it supports transparency.
To allow the user to have the most flexibility, various transparency modes have been created.

There are three transparency states:
   - `automatic`: uses a flag in the gif file to deduce if the gif is transparent.
   - `force`: forces transparancy regardless of the flag.
   - `off`: does not use transparency and instead uses gif background color.
   
 There are two tranparency modes:
   - `dynamic`: uses a 0-255 transparency value for each pixel. May not work well with some images.
   - `boolean`: uses either a 0 or 255 transparency value for each pixel. This makes the background transparent without touching the image pixels.
   
 Thus, there are four available transparency settings:
   - `Gif::Transparency::automatic`: (default) uses dynamic transparency mode only if transparency flag is set in the gif file.
   - `Gif::Transparency::dynamic`: (force) enables dynamic transparency mode where transparency is range of 0-100%.
   - `Gif::Transparency::boolean`: (force) enables boolean transparency mode where transparency is either 0% or 100%.
   - `Gif::Transparency::off`: disables transparency and uses gif background color, sometimes white.

For best results, try `Gif::Transparency::boolean` if you want transparency, or `Gif::Transparency::off` if you want the GIF to be unaltered by transparency.

### LVGL Integration
To move, resize, or change the background color, create an [LVGL object](https://docs.littlevgl.com/en/html/object-types/obj.html) to contain the GIF:
```cpp
lv_obj_t* obj = lv_obj_create(lv_scr_act(), NULL);
lv_obj_set_size(obj, 200, 200);
lv_obj_set_style(obj, &lv_style_plain_color);
lv_obj_align(obj, NULL, LV_ALIGN_CENTER, 0, 0);

Gif gif("/usd/mygif.gif", obj, Gif::Transparency::automatic);
```

### Final Notes
To learn more about gif-pros, see the header file documentation in `/include/gifclass.hpp`.

An example program is in `/src/opcontrol.cpp`.

This library uses the MIT lisence, so feel free to use it in your programs as long as it credits this repository.