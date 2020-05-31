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

    // Parse stdin
    double duration_s, time_left_s;
    if (argc < 2) {
        fprintf(stderr, "brbtimer: duration parameter required (in seconds)\n");
        return 1;
    } else if (argc > 2) {
        fprintf(stderr, "brbtimer: only one parameter required (duration)\n");
        return 1;
    } else {
        char *p;
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

    // Load sprites
    ALLEGRO_BITMAP *spr_track, *anim_run[6], *anim_finish[4];
    spr_track  = al_load_bitmap("res/track.png");
    for (int i = 0; i < 6; i++) {
        char filename[13], suffix[6];
        strcpy(filename, "res/run");
        sprintf(suffix, "%d.png", i + 1);
        strcat(filename, suffix);
        anim_run[i] = al_load_bitmap(filename);
    }
    for (int i = 0; i < 4; i++) {
        char filename[16], suffix[6];
        strcpy(filename, "res/finish");
        sprintf(suffix, "%d.png", i + 1);
        strcat(filename, suffix);
        anim_finish[i] = al_load_bitmap(filename);
    }
    // Store runner start and end coordinates
    float RUN_START_X, RUN_FINISH_X;
    RUN_START_X = 6.0;
    RUN_FINISH_X = DISPLAY_WIDTH - 4.5 - al_get_bitmap_width(anim_run[0]);

    /* END OF INITIALIZATION */

    state = RUNNING;
    do {
        // Read input
        
        // Act
        time_left_s = (time_left_s > 0.0)? time_left_s - 1.0 : time_left_s;
        float track_x, track_y;
        track_x = (DISPLAY_WIDTH - al_get_bitmap_width(spr_track)) / 2.0;
        track_y = (DISPLAY_HEIGHT - al_get_bitmap_height(spr_track)) / 2.0;
        float progress = (duration_s - time_left_s) / duration_s;
        float run_x;
        run_x = (RUN_FINISH_X - ((time_left_s / duration_s) * (RUN_FINISH_X - RUN_START_X)));

        // Draw
        al_clear_to_color(BACKGROUND_COLOR);
        al_draw_bitmap(spr_track, track_x, track_y, 0);
        al_draw_bitmap(anim_run[0], run_x, 13.0, 0);
        al_flip_display();
    } while (state != SHUTDOWN);

    return 0;
}
