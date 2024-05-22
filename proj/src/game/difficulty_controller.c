#include <lcom/lcf.h>

#include "difficulty_controller.h"

extern int score;
extern int freq;

extern int a_speed;
extern int a_diagspeed;

bool change_difficulty = false;

void update_difficulty() {
    switch (score) {
        case 10:
            if (!change_difficulty) {
                printf("Level 1\n");
                change_difficulty = true;
                freq -= 5;
            }
            break;
        case 20:
            if (change_difficulty) {
                printf("Level 2\n");
                change_difficulty = false;
                freq -= 5;
            }
            break;
        case 30:
            if (!change_difficulty) {
                printf("Level 3\n");
                change_difficulty = true;
                freq -= 3;
                a_speed++;
                a_diagspeed++;
            }
            break;
        case 40:
            if (change_difficulty) {
                printf("Level 4\n");
                change_difficulty = false;
                freq -= 2;
                a_speed++;
                a_diagspeed++;
            }
            break;
        case 50:
            if (!change_difficulty) {
                printf("Level 5\n");
                change_difficulty = true;
                freq -= 3;
                a_speed++;
                a_diagspeed++;
            }
            break;
        case 60:
            if (change_difficulty) {
                printf("Level 6\n");
                change_difficulty = false;
                freq -= 2;
                a_speed++;
                a_diagspeed++;
            }
            break;
        case 70:
            if (!change_difficulty) {
                printf("Level 7\n");
                change_difficulty = true;
                freq -= 3;
                a_speed++;
                a_diagspeed++;
            }
            break;
        case 80:
            if (change_difficulty) {
                printf("Level 8\n");
                change_difficulty = false;
                freq -= 2;
                a_speed++;
                a_diagspeed++;
            }
            break;
    }
}
