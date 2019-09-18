#include "gifclass.hpp"

Gif::Gif(const char* fname, lv_obj_t* parent, Transparency mode) {
	_mode = mode;
	FILE* fp = fopen(fname, "rb");

	if(fp != NULL) {
		fseek(fp, 0, SEEK_END);
		size_t len = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		_gifmem = malloc(len);

		if(_gifmem != NULL) {
			fread(_gifmem, 1, len, fp);
		} else {
			std::cerr << "Gif::Gif - not enough memory for GIF" << std::endl;
		}
		fclose(fp);

		if(_gifmem != NULL) {
			// create a FILE from memory buffer
			FILE* memfp = fmemopen(_gifmem, len, "rb");

			// open gof file
			// will allocate memory for background and one animation frame.
			_gif = gd_open_gif(memfp);
			if(_gif == NULL) {
				std::cerr << "Gif::Gif - unable to allocate memory for one animation" << std::endl;
				return;
			}

			// memory for rendering frame
			_buffer = (uint8_t*)malloc(_gif->width * _gif->height * BYTES_PER_PIXEL);
			if(_buffer == NULL) {
				// out of memory
				_cleanup();
				std::cerr << "Gif::Gif - not enough memory for frame buffer" << std::endl;
			} else {
				_cbuf = new lv_color_t[_gif->width * _gif->height];
				_canvas = lv_canvas_create(parent, NULL);
				lv_canvas_set_buffer(_canvas, _cbuf, _gif->width, _gif->height, LV_IMG_CF_TRUE_COLOR_ALPHA);
				_task = pros::c::task_create(_render_task, static_cast<void*>(this), TASK_PRIORITY_DEFAULT-1, TASK_STACK_DEPTH_DEFAULT, "GIF");
			}
		}
	} else {
		std::cerr << "Gif::Gif - unable to open gif (file not found)" << std::endl;
	}

};


Gif::~Gif() {
	_cleanup();
}


void Gif::_cleanup() {
	pros::c::task_delete(_task);
	lv_obj_del(_canvas);
	delete[] _cbuf; _cbuf = nullptr;
	free(_buffer); _buffer = nullptr;
	gd_close_gif(_gif);
	free(_gifmem); _gifmem = nullptr;
}


void Gif::_render() {

	for (unsigned looped = 1;; looped++) {
		while (gd_get_frame(_gif)) {
			int32_t now = pros::millis();

			gd_render_frame(_gif, _buffer);

			for (size_t i = 0; i < _gif->height * _gif->width; i++) {
				_cbuf[i].red = _buffer[(i * BYTES_PER_PIXEL)];
				_cbuf[i].green = _buffer[(i * BYTES_PER_PIXEL) + 1];
				_cbuf[i].blue = _buffer[(i * BYTES_PER_PIXEL) + 2];

				uint8_t transparency = _buffer[(i * BYTES_PER_PIXEL) + 3];

				switch(_mode) {
					case Gif::Transparency::automatic: {
						// if transparency is not enabled in gif, make all pixles non-transparent
						transparency = !_gif->gce.transparency ? 255 : transparency;
						break;
					}
					case Gif::Transparency::dynamic: {
						// don't need to change anything
						break;
					}
					case Gif::Transparency::boolean: {
						// only fully-transparent pixels stay transparent
						transparency = transparency != 0 ? 255 : 0;
						break;
					}
					case Gif::Transparency::off: {
						// make all pixles non-transparent
						transparency = 255;
						break;
					}
				}

				_cbuf[i].alpha = transparency;
			};

			lv_obj_invalidate(_canvas);

			int32_t delay = _gif->gce.delay * 10;
			int32_t delta = pros::millis() - now;
			delay -= delta;

			if(delay > 0) pros::delay(delay);
		}

		if(looped == _gif->loop_count) break;
		gd_rewind(_gif);
	}

	_cleanup();
}


void Gif::_render_task(void* arg) {
	Gif* instance = static_cast<Gif*>(arg);

	while(true) {
		instance->_render();
		pros::delay(2);
	}
}