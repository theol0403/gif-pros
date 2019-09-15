#include "gifclass.hpp"


Gif::Gif( const char *fname, int sx, int sy ) {
	_sx = sx;
	_sy = sy;
	FILE *fp = fopen( fname, "rb" );

	if( fp != NULL ) {
		fseek( fp, 0, SEEK_END );
		size_t len = ftell( fp );
		fseek( fp, 0, SEEK_SET );

		_gifmem = malloc( len );

		if( _gifmem != NULL ) {
			int nRead = fread( _gifmem, 1, len, fp );
			(void) nRead;
		} else {
			std::cerr << "Gif::Gif - not enough memory for GIF";
		}
		fclose(fp);

		if( _gifmem != NULL ) {
			// create a FILE from memory buffer
			FILE *fp = fmemopen( _gifmem, len, "rb" );

			// open gof file
			// will allocate memory for background and one animation
			// frame.
			_gif = gd_open_gif( fp );
			if( _gif == NULL ) {
				std::cerr << "Gif::Gif - unable to allocate memory for one animation";
				return;
			}

			// memory for rendering frame
			_buffer = (uint32_t *)malloc(_gif->width * _gif->height * sizeof(uint32_t));
			if( _buffer == NULL ) {
				// out of memory
				gd_close_gif( _gif );
				free(_gifmem);
				std::cerr << "Gif::Gif - not enough memory for frame buffer";
			} else {
				// create thread to handle this gif
				_task = pros::c::task_create( render, static_cast<void *>(this), TASK_PRIORITY_DEFAULT-1, TASK_STACK_DEPTH_DEFAULT, "GIF");
			}
		}
	}

};


Gif::~Gif() {
	pros::c::task_delete(_task);
	free(_buffer);
	gd_close_gif( _gif );
	free(_gifmem);
}


extern "C" {
	void vexDisplayCopyRect( int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t *pSrc, int32_t srcStride );
}

// static void copy_rect


void Gif::render(void *arg ) {
	if( arg == NULL) {
		std::cerr << "Gif::render - arg is NULL";
		return;
	}

	Gif *instance = static_cast<Gif*>(arg);

	gd_GIF *gif = instance->_gif;

	for (unsigned looped = 1;; looped++) {
		while (gd_get_frame(gif)) {
			int32_t now = pros::millis();

			gd_render_frame(gif, (uint8_t*)instance->_buffer);

			int32_t ex = instance->_sx + gif->width - 1;
			int32_t ey = instance->_sy + gif->height - 1;
			ex  = (ex  > 479 ? 479 : ex  );
			ey  = (ey  > 239 ? 239 : ey  );

			vexDisplayCopyRect( instance->_sx, instance->_sy, ex, ey, (uint32_t*)instance->_buffer, gif->width);

			int32_t delay = gif->gce.delay * 10;

			int32_t delta = pros::millis() - now;
			delay -= delta;
			
			if( delay > 0 ) pros::delay(delay);
		}

		if (looped == gif->loop_count) break;

		gd_rewind(gif);
	}

	free(instance->_buffer);
	gd_close_gif(gif);
	free(instance->_gifmem);
}