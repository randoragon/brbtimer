#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "brbtimer.h"

#define FPS 60
#define ANIMATION_FPS 6
#define BASE_WIDTH 140
#define BASE_HEIGHT 52
#define DISPLAY_SCALE 3
#define DISPLAY_WIDTH (BASE_WIDTH * DISPLAY_SCALE)
#define DISPLAY_HEIGHT (BASE_HEIGHT * DISPLAY_SCALE)

void help()
{
    printf(
"SYNOPSIS\n\
    brbtimer [-h, --help] [-n, --noconfirm] duration...\n\
\n\
    Duration must be a natural number, the implicit unit is seconds.\n\
    You can use the 'h', 'm' and 's' suffixes to specify hours, minutes and\n\
    seconds respectively.\n\
\n\
    If you pass multiple duration parameters, their sum will be calculated.\n\
\n\
    Note that brbtimer was coded with simple purposes in mind, and does not\n\
    run extensive checks for integer overflow. But it shouldn't matter so long\n\
    you use it reasonably (don't expect setting it to 99999999 hours to work).\n\
\n\
OPTIONS\n\
    -h, --help\n\
        Prints this help message.\n\
\n\
    -n, --noconfirm\n\
        Starts the timer without waiting for the user to press enter.\n\
\n\
EXAMPLES\n\
    brbtimer 30         - sets the timer to 30 seconds\n\
    brbtimer 20m        - sets the timer to 20 minutes\n\
    brbtimer 1h 5m 2 3  - sets the timer to 1 hour, 5 minutes and 5 seconds\n\
\n\
AUTHOR\n\
    Written by Randoragon <randoragongamedev@gmail.com>\n\
    Browse the source code at https://github.com/randoragon/brbtimer\n\
");
}

int main(int argc, char **argv)
{
    /* INITIALIZATION PHASE */

    state_t state = INIT;

    // Parse stdin
    unsigned int duration, frames_left;
    bool noconfirm = false;
    duration = 0;
    if (argc < 2) {
        fprintf(stderr, "brbtimer: at least one parameter required (try 'brbtimer --help')\n");
        return 1;
    } else {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            help();
            return 0;
        } else if (strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "--noconfirm") == 0) {
            noconfirm = true;
            if (argc < 3) {
                fprintf(stderr, "brbtimer: at least one duration parameter required (try 'brbtimer --help')\n");
                return 1;
            } else {
                // Shift all parameters backwards by one place
                // Note that by doing this, we lose the argv[0] executable path,
                // but that's okay, because it's not used anywhere in the code.
                argv += 1;
                argc -= 1;
            }
        }

        // Loop over duration parameters and add them up
        for (int i = 1; i < argc; i++) {
            char *p;
            unsigned int amount, unit;
            bool bad_syntax = false;
            amount = strtoul(argv[i], &p, 10) * FPS;
            if (argv[i][0] == '-' || amount == 0 || errno == ERANGE) {
                fprintf(stderr, "brbtimer: failed to parse parameters, please use numbers between 1 and %u\n", UINT_MAX);
                return 1;
            } else if ((*p != 0 && *(p+1) != 0) || (*(p-1) < '0' || *(p-1) > '9')) {
                bad_syntax = true;
            } else switch (*p) {
                case 0: case 's': unit = 1; break;
                case 'h': unit = 3600; break;
                case 'm': unit = 60; break;
                default: bad_syntax = true; break;
            }
            if (bad_syntax) {
                fprintf(stderr, "brbtimer: failed to parse parameters (try 'brbtimer --help')\n");
                return 1;
            }
            duration += amount * unit;
        }
        frames_left = duration;
    }

    // Initialize library components
    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();

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

    // Load font
    ALLEGRO_FONT *pixeldise = al_load_ttf_font("res/pixeldise.ttf", -12 * 2 * DISPLAY_SCALE, ALLEGRO_TTF_NO_KERNING | ALLEGRO_TTF_MONOCHROME | ALLEGRO_TTF_NO_AUTOHINT);

    // Supplementary animation variables
    int total_frames = 0;
    float run_start_x, run_finish_x;
    run_start_x = 6.0 * DISPLAY_SCALE;
    run_finish_x = DISPLAY_WIDTH - ((4.5 + anim_run_w) * DISPLAY_SCALE);

    /* END OF INITIALIZATION */

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    state = noconfirm? RUNNING : WAITING;
    ALLEGRO_EVENT event;
    bool fps_step;
    al_start_timer(timer);

    do {
        al_wait_for_event(event_queue, &event);
        fps_step = (event.type == ALLEGRO_EVENT_TIMER);

        // Read input
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_Q) {
                // For some reason Allegro 5 does not send keyboard modifiers properly
                // to the KEY_DOWN event (see https://www.allegro.cc/forums/thread/607395).
                // As a workaround, you can check if ctrl is down using keyboard states.
                ALLEGRO_KEYBOARD_STATE key_state;
                al_get_keyboard_state(&key_state);
                if (al_key_down(&key_state, ALLEGRO_KEY_LCTRL) || al_key_down(&key_state, ALLEGRO_KEY_RCTRL)) {
                    state = SHUTDOWN;
                }
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                if (state == WAITING) {
                    state = RUNNING;
                } else if (state == RUNNING && frames_left == 0) {
                    state = SHUTDOWN;
                }
            }
        }
        
        if (fps_step) {
            // Compute Step
            if (state == RUNNING) {
                frames_left = (frames_left > 0)? frames_left - 1 : frames_left;
                total_frames += 1;
            }
            float track_x, track_y;
            track_x = (DISPLAY_WIDTH  - (spr_track_w * DISPLAY_SCALE)) / 2;
            track_y = (DISPLAY_HEIGHT - (spr_track_h * DISPLAY_SCALE)) / 2;
            float run_x;
            run_x = (run_finish_x - (((float)frames_left / duration) * (run_finish_x - run_start_x)));
            int anim_frame;
            anim_frame = (int)((float)total_frames / ((float)FPS / ANIMATION_FPS));
            int h, m, s;
            h = (frames_left + FPS) / (3600 * FPS);
            m = ((frames_left + FPS) % (3600 * FPS)) / (60 * FPS);
            s = (frames_left + FPS) % (60 * FPS) / FPS;

            // Draw Results
            al_clear_to_color(BACKGROUND_COLOR);
            if (state == RUNNING && frames_left != 0) {
                char mstr[3], sstr[3];
                sprintf(mstr, "%2d", m);
                sprintf(sstr, "%2d", s);
                for (int i = 0; i < 3; i++) {
                    mstr[i] = (mstr[i] == ' ')? '0' : mstr[i];
                    sstr[i] = (sstr[i] == ' ')? '0' : sstr[i];
                }
                if (h > 0) {
                    al_draw_textf(pixeldise, al_map_rgb(255, 255, 255), (DISPLAY_WIDTH / 2) - (14 * DISPLAY_SCALE), 29 * DISPLAY_SCALE, ALLEGRO_ALIGN_RIGHT, "%d", h);
                    al_draw_text(pixeldise, al_map_rgb(255, 255, 255), (DISPLAY_WIDTH / 2) - (12 * DISPLAY_SCALE), 28 * DISPLAY_SCALE, ALLEGRO_ALIGN_CENTER, ":");
                    al_draw_textf(pixeldise, al_map_rgb(255, 255, 255), DISPLAY_WIDTH / 2, 29 * DISPLAY_SCALE, ALLEGRO_ALIGN_CENTER, "%s", mstr);
                    al_draw_text(pixeldise, al_map_rgb(255, 255, 255), (DISPLAY_WIDTH / 2) + (12 * DISPLAY_SCALE), 28 * DISPLAY_SCALE, ALLEGRO_ALIGN_CENTER, ":");
                    al_draw_textf(pixeldise, al_map_rgb(255, 255, 255), (DISPLAY_WIDTH / 2) + (14 * DISPLAY_SCALE), 29 * DISPLAY_SCALE, ALLEGRO_ALIGN_LEFT, "%s", sstr);
                } else if (m > 0 || s == 0) {
                    al_draw_textf(pixeldise, al_map_rgb(255, 255, 255), (DISPLAY_WIDTH / 2) - (2 * DISPLAY_SCALE), 29 * DISPLAY_SCALE, ALLEGRO_ALIGN_RIGHT, "%d", m);
                    al_draw_text(pixeldise, al_map_rgb(255, 255, 255), DISPLAY_WIDTH / 2, 28 * DISPLAY_SCALE, ALLEGRO_ALIGN_CENTER, ":");
                    al_draw_textf(pixeldise, al_map_rgb(255, 255, 255), (DISPLAY_WIDTH / 2) + (2 * DISPLAY_SCALE), 29 * DISPLAY_SCALE, ALLEGRO_ALIGN_LEFT, "%s", sstr);
                } else {
                    al_draw_textf(pixeldise, al_map_rgb(255, 255, 255), DISPLAY_WIDTH / 2, 29 * DISPLAY_SCALE, ALLEGRO_ALIGN_CENTER, "%s", sstr);
                }
            }
            al_draw_scaled_bitmap(spr_track, 0, 0, spr_track_w, spr_track_h, track_x, track_y, spr_track_w * DISPLAY_SCALE, spr_track_h * DISPLAY_SCALE, 0);
            if (state == RUNNING) {
                if (frames_left != 0) {
                    al_draw_scaled_bitmap(anim_run[anim_frame % 6], 0, 0, anim_run_w, anim_run_h, run_x, 13 * DISPLAY_SCALE, anim_run_w * DISPLAY_SCALE, anim_run_h * DISPLAY_SCALE, 0);
                } else {
                    al_draw_scaled_bitmap(anim_finish[anim_frame % 4], 0, 0, anim_finish_w, anim_finish_h, run_x, 13 * DISPLAY_SCALE, anim_finish_w * DISPLAY_SCALE, anim_finish_h * DISPLAY_SCALE, 0);
                }
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
    al_destroy_font(pixeldise);

    return 0;
}
