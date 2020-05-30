#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "brbtimer.h"

int main(int argc, char **argv)
{
    /* INITIALIZATION PHASE */

    state_t state = INIT;

    // Validate input
    if (argc != 2) {
        fprintf(stderr, "brbtimer: duration parameter required (in seconds)\n");
        return 1;
    } else {
        char *p;
        unsigned int duration_s, time_left_s;
        duration_s = strtoul(argv[1], &p, 10);
        if (argv[1][0] == '-' || *p != 0 || errno == ERANGE) {
            fprintf(stderr, "brbtimer: duration must be a number of seconds between 0 and %u\n", UINT_MAX);
            return 1;
        }
        time_left_s = duration_s;
    }

    // Initialize library components
    al_init();
    al_init_image_addon();

    // Create and configure display
    unsigned int DISPLAY_WIDTH, DISPLAY_HEIGHT;
    DISPLAY_WIDTH = 140;
    DISPLAY_HEIGHT = 52;
    ALLEGRO_COLOR BACKGROUND_COLOR = al_map_rgb(255, 0, 255);
    ALLEGRO_DISPLAY *display;
    display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    al_clear_to_color(BACKGROUND_COLOR);

    // Load sprites
    ALLEGRO_BITMAP *spr_track  = al_load_bitmap("res/track.png");
    ALLEGRO_BITMAP *spr_run    = al_load_bitmap("res/run.gif");
    ALLEGRO_BITMAP *spr_finish = al_load_bitmap("res/finish.gif");

    /* END OF INITIALIZATION */

    state = RUNNING;
    do {
        // Read input
        
        // Act
        float track_x, track_y;
        track_x = (DISPLAY_WIDTH - al_get_bitmap_width(spr_track)) / 2.0;
        track_y = (DISPLAY_HEIGHT - al_get_bitmap_height(spr_track)) / 2.0;

        // Redraw
        al_draw_bitmap(spr_track, track_x, track_y, 0);
        al_flip_display();
    } while (state != SHUTDOWN);

    return 0;
}
