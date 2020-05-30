#include <allegro5/allegro.h>

extern ALLEGRO_DISPLAY *display;

typedef enum state {
    INIT,
    RUNNING,
    SHUTDOWN
} state_t;
extern state_t state;

extern unsigned int duration_s, time_left_s;
