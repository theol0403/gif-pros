#include "gifclass.hpp"

Gif::Gif( const char *fname, lv_obj_t* parent, int sx, int sy ) {
	_sx = sx;
	_sy = sy;
	FILE *fp = fopen(fname, "rb");

	if( fp != NULL ) {
		fseek(fp, 0, SEEK_END);
		size_t len = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		_gifmem = malloc(len);

		if(_gifmem != NULL) {
			int nRead = fread(_gifmem, 1, len, fp);
			(void) nRead;
		} else {
			std::cerr << "Gif::Gif - not enough memory for GIF" << std::endl;
		}
		fclose(fp);

		if(_gifmem != NULL) {
			// create a FILE from memory buffer
			FILE *fp = fmemopen( _gifmem, len, "rb" );

			// open gof file
			// will allocate memory for background and one animation frame.
			_gif = gd_open_gif(fp);
			if(_gif == NULL) {
				std::cerr << "Gif::Gif - unable to allocate memory for one animation" << std::endl;
				return;
			}

			// memory for rendering frame
			_buffer = (uint8_t*)malloc(_gif->width * _gif->height * sizeof(uint8_t));
			if(_buffer == NULL) {
				// out of memory
				gd_close_gif(_gif);
				free(_gifmem);
				std::cerr << "Gif::Gif - not enough memory for frame buffer" << std::endl;
			} else {
				_cbuf = new lv_color_t[_gif->width * _gif->height];
        _convertbuf = new lv_color_t[_gif->width * _gif->height];
        _canvas = lv_canvas_create(parent, NULL);
        lv_canvas_set_buffer(_canvas, _cbuf, _gif->width, _gif->height, LV_IMG_CF_TRUE_COLOR);
        _task = pros::c::task_create(_render_task, static_cast<void*>(this), TASK_PRIORITY_DEFAULT-1, TASK_STACK_DEPTH_DEFAULT, "GIF");
      }
    }
  } else {
    std::cerr << "Gif::Gif - unable to open gif (file not found)" << std::endl;
  }

};


Gif::~Gif() {
	pros::c::task_delete(_task);
	lv_obj_del(_canvas);
	delete[] _cbuf;
	free(_buffer);
	gd_close_gif( _gif );
	free(_gifmem);
}


void Gif::_render(void *arg ) {
	if( arg == NULL) {
		std::cerr << "Gif::_render - arg is NULL" << std::endl;
		return;
	}

	Gif *instance = static_cast<Gif*>(arg);

	gd_GIF *gif = instance->_gif;

	for (unsigned looped = 1;; looped++) {
		while (gd_get_frame(gif)) {
			int32_t now = pros::millis();

			gd_render_frame(gif, (uint8_t*)instance->_buffer);

      for (int i = 0; i < gif->height * gif->width; i++) {
        instance->_convertbuf[i].blue = ((uint8_t*)instance->_buffer)[(i * 4)];
        instance->_convertbuf[i].green = ((uint8_t*)instance->_buffer)[(i * 4) + 1];
        instance->_convertbuf[i].red = ((uint8_t*)instance->_buffer)[(i * 4) + 2];
        instance->_convertbuf[i].alpha = ((uint8_t*)instance->_buffer)[(i * 4) + 3];
      };

      lv_canvas_set_buffer(instance->_canvas, instance->_convertbuf, gif->width, gif->height, LV_IMG_CF_TRUE_COLOR);
      // lv_canvas_copy_buf(instance->_canvas, instance->_convertbuf, instance->_sx, instance->_sy, gif->width-1, gif->height-1);

      int32_t delay = gif->gce.delay * 10;

      int32_t delta = pros::millis() - now;
      delay -= delta;

      if(delay > 0) pros::delay(delay);
    }

    if (looped == gif->loop_count) break;

    gd_rewind(gif);
  }

  //destruct object and free memory
  instance->~Gif();
}


void Gif::_render_task(void *arg) {
	while(true) {
		Gif::_render(arg);
		pros::delay(2);
	}
}