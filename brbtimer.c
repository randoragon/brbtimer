#include <allegro5/allegro.h>
#include "event.h"

int main()
{
    int end_game;

    al_init();
    init();

    end_game = 0;
    do {
        input();
        process();
        output();
    } while (end_game == 0);

    shutdown();
    return 0;
}
