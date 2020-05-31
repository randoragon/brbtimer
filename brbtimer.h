#include <allegro5/allegro.h>

extern ALLEGRO_DISPLAY *display;

typedef enum state {
    INIT,
    WAITING,
    RUNNING,
    SHUTDOWN
} state_t;
extern state_t state;

extern unsigned int duration, frames_left;

void help();
