#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "brbtimer.h"

#define FPS 60
#define ANIMATION_FPS 6
#define BASE_WIDTH 140
#define BASE_HEIGHT 52
#define DISPLAY_SCALE 2
#define DISPLAY_WIDTH (BASE_WIDTH * DISPLAY_SCALE)
#define DISPLAY_HEIGHT (BASE_HEIGHT * DISPLAY_SCALE)

int main(int argc, char **argv)
{
    /* INITIALIZATION PHASE */

    state_t state = INIT;

    // Parse stdin
    unsigned int duration, frames_left;
    if (argc < 2) {
        fprintf(stderr, "brbtimer: duration parameter required (in seconds)\n");
        return 1;
    } else if (argc > 2) {
        fprintf(stderr, "brbtimer: only one parameter required (duration)\n");
        return 1;
    } else {
        char *p;
        duration = strtoul(argv[1], &p, 10) * FPS;
        if (argv[1][0] == '-' || *p != 0 || errno == ERANGE) {
            fprintf(stderr, "brbtimer: duration must be a number of seconds between 0 and %u\n", UINT_MAX);
            return 1;
        }
        frames_left = duration;
    }

    // Initialize library components
    al_init();
    al_init_image_addon();

    // Create and configure display
    const ALLEGRO_COLOR BACKGROUND_COLOR = al_map_rgb(255, 0, 255);
    ALLEGRO_DISPLAY *display;
    display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

    // Load sprites
    ALLEGRO_BITMAP *spr_track, *anim_run[6], *anim_finish[4];
    int spr_track_w, spr_track_h, anim_run_w, anim_run_h, anim_finish_w, anim_finish_h;
    spr_track  = al_load_bitmap("res/track.png");
    spr_track_w = al_get_bitmap_width(spr_track);
    spr_track_h = al_get_bitmap_height(spr_track);
    for (int i = 0; i < 6; i++) {
        char filename[13], suffix[6];
        strcpy(filename, "res/run");
        sprintf(suffix, "%d.png", i + 1);
        strcat(filename, suffix);
        anim_run[i] = al_load_bitmap(filename);
    }
    anim_run_w = al_get_bitmap_width(anim_run[0]);
    anim_run_h = al_get_bitmap_height(anim_run[0]);
    for (int i = 0; i < 4; i++) {
        char filename[16], suffix[6];
        strcpy(filename, "res/finish");
        sprintf(suffix, "%d.png", i + 1);
        strcat(filename, suffix);
        anim_finish[i] = al_load_bitmap(filename);
    }
    anim_finish_w = al_get_bitmap_width(anim_finish[0]);
    anim_finish_h = al_get_bitmap_height(anim_finish[0]);

    // Supplementary animation variables
    int total_frames = 0;
    float run_start_x, run_finish_x;
    run_start_x = 6.0 * DISPLAY_SCALE;
    run_finish_x = DISPLAY_WIDTH - ((4.5 + anim_run_w) * DISPLAY_SCALE);

    /* END OF INITIALIZATION */

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    state = RUNNING;
    ALLEGRO_EVENT event;
    bool fps_step;
    al_start_timer(timer);

    do {
        al_wait_for_event(event_queue, &event);
        fps_step = (event.type == ALLEGRO_EVENT_TIMER);

        // Read input
        
        if (fps_step) {
            // Compute Step
            frames_left = (frames_left > 0)? frames_left - 1 : frames_left;
            float track_x, track_y;
            track_x = (DISPLAY_WIDTH  - (spr_track_w * DISPLAY_SCALE)) / 2;
            track_y = (DISPLAY_HEIGHT - (spr_track_h * DISPLAY_SCALE)) / 2;
            float run_x;
            run_x = (run_finish_x - (((float)frames_left / duration) * (run_finish_x - run_start_x)));
            int anim_frame;
            anim_frame = (int)((float)total_frames++ / ((float)FPS / ANIMATION_FPS));

            // Draw Results
            al_clear_to_color(BACKGROUND_COLOR);
            al_draw_scaled_bitmap(spr_track, 0, 0, spr_track_w, spr_track_h, track_x, track_y, spr_track_w * DISPLAY_SCALE, spr_track_h * DISPLAY_SCALE, 0);
            if (frames_left != 0) {
                al_draw_scaled_bitmap(anim_run[anim_frame % 6], 0, 0, anim_run_w, anim_run_h, run_x, 13 * DISPLAY_SCALE, anim_run_w * DISPLAY_SCALE, anim_run_h * DISPLAY_SCALE, 0);
            } else {
                al_draw_scaled_bitmap(anim_finish[anim_frame % 4], 0, 0, anim_finish_w, anim_finish_h, run_x, 13 * DISPLAY_SCALE, anim_finish_w * DISPLAY_SCALE, anim_finish_h * DISPLAY_SCALE, 0);
            }
            al_flip_display();
        }
    } while (state != SHUTDOWN);

    // Cleanup
    al_destroy_bitmap(spr_track);
    for (int i = 0; i < 6; i++) {
        al_destroy_bitmap(anim_run[i]);
    }
    for (int i = 0; i < 4; i++) {
        al_destroy_bitmap(anim_finish[i]);
    }
    al_destroy_event_queue(event_queue);

    return 0;
}
