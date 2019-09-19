# gif-pros
PROS GIF decoder with LVGL integration and transparency support.

![gif-pros](https://github.com/theol0403/gif-pros/raw/master/gif-pros.png)


Loosely based on jpearman's [VEXcode GIF demo](https://www.vexforum.com/t/animated-gif-demo-vexcode/58755).
Inspired by [VEXForum request](https://www.vexforum.com/t/gif-in-pros/66187?u=theol0403).
Implements [`gifdec.c`](https://github.com/lecram/gifdec).

gif-pros supports transparency and implements the LVGL canvas, which makes it extremely flexible and versatile.


# Installation in external project
### Use pre-built template
Instead of building the project yourself you can use a pre-built template to apply to your external project.
Download `gif-pros@<version>.zip`</a> from the repository and then fetch into PROS as described below.
### Building
To build the template yourself, clone the repository and run `prosv5 make template`.
### Apply template to project 
Now that you have a template, run `prosv5 c fetch <template>` to import the template into PROS.

Finally, run `prosv5 c apply gif-pros` in your destination project to install the package.
You can now use `Gif` in your project, after you include the header:
```cpp
#include "gif-pros/gifclass.h"
```

# Use as standalone project
You can also use this repository as a standalone project and run it directly on the robot.
Build by running `prosv5 make`.
There is an example program in `src/initialize.cpp`.

# Use
After you have included the header file, run 
```cpp
Gif gif("/usd/mygif.gif", 120, 0 );
```
Note the `/usd/` prefix, which is used in PROS to signify the SD card.
