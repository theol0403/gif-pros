# gif-pros
A GIF decoder written for PROS with LVGL integration and transparency support.

![gif-pros](https://github.com/theol0403/gif-pros/raw/master/gif-pros.png)


Loosely based on jpearman's [VEXcode GIF demo](https://www.vexforum.com/t/animated-gif-demo-vexcode/58755).
Inspired by [this](https://www.vexforum.com/t/gif-in-pros/66187?u=theol0403) VEXForum request.
Implements the [gifdec.c](https://github.com/lecram/gifdec) decoder.

This library supports transparency and implements the LVGL canvas, making it extremely flexible and versatile to use.

# Installation
#### 1a. Pre-built template
To get the template, head over to the [releases](https://github.com/theol0403/gif-pros/releases) page and download `gif-pros@<version>.zip`.
**Note:** If you are still using PROS 3, download version 2.1.1. If you are using PROS 4 with LVGL 8.3, download version 3 or higher.
#### 1b. Build
To build the template yourself instead of downloading, clone this repository and run `pros make template`.
#### 2. Fetch template
Run `pros c fetch <template>` to import the template into the CLI.
#### 3. Apply template
Run `pros c apply gif-pros` in your destination project to install the package.

**Note:** PROS kernel version 3.2.0 or higher is required.

## Standalone project
You can also use this repository as a standalone project and run it directly on the robot.
There is an example program in `src/main.cpp`.

# Use
After you have installed gif-pros into your project, include the header:
```cpp
#include "gif-pros/gifclass.hpp"
```
To initialize the GIF reader, run this command:
```cpp
/**
 * Construct the Gif class
 * @param fname  the gif filename on the SD card (prefixed with /usd/)
 * @param parent the LVGL parent object
 */
Gif gif("/usd/mygif.gif", lv_scr_act());
```

**Note:** The `Gif` object cannot go out of scope or be destroyed. If you need the object to go out of scope, you can use `static` or dynamically create the object using `new`:
```cpp
// this block can go out of scope
static Gif gif("/usd/mygif.gif", lv_scr_act());
// or
Gif* gif = new Gif("/usd/mygif.gif", lv_scr_act()); 
```

### LVGL Integration
To move or resize the gif or to change the background color, create an [LVGL object](https://docs.littlevgl.com/en/html/object-types/obj.html) to manipulate the GIF:
```cpp
lv_obj_t* obj = lv_obj_create(lv_scr_act(), NULL);
lv_obj_set_size(obj, 480, 240);
lv_obj_set_style(obj, &lv_style_transp); // make the container invisible
lv_obj_align(obj, NULL, LV_ALIGN_CENTER, 0, 0);

Gif gif("/usd/mygif.gif", obj);
```
To change the background color or add borders to the GIF, simply edit the LVGL style.

### Gif Settings
Displaying GIFs is a fairly CPU-intensive operation. To increase preformance by making the file smaller, optimize the GIF by using this [online tool](https://ezgif.com/optimize/). If your gif only contains a few colors, make sure to optimize the color palate using the "Color Reduction" option.

**Note:** gif-pros only works with GIFs that have global color tables. To add one, select the "Use single color table" option of the GIF optimizer.

If you want the GIF to only loop once, use this [online tool](https://ezgif.com/loop-count/) to set the loop flag.

[ezgif.com](https://ezgif.com/effects) also offers many other tools to resize or edit GIF animations, so be sure to check it out!

### Removing GIFs
GIFs take a lot of memory, so it is important to delete them when not in use. They will automatically self-delete if the loop count (if any) has been reached, or if the Gif object is destructed, but otherwise the GIF will loop forever.
To delete the GIF and free the memory, call:
```cpp
gif.clean();
```
**Note:** Currently, to re-open the GIF you need to create a new object. I might implement a feature that lets you open and close without re-creating the object.

### Final Notes
To learn more about gif-pros, see the header file documentation in `/include/gifclass.hpp`.
An example program is in `/src/main.cpp`.

This library uses the MIT lisence, so feel free to use it in your programs as long as it credits this repository.
