#ifndef __allegro_wrapper_h
#define __allegro_wrapper_h

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "error.h"

typedef enum {DISPLAY, EVENT_QUEUE, TIMER, BITMAP, SAMPLE, FONT} resource;

#endif
