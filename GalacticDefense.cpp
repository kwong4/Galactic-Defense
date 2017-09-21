#include <stdio.h>
#include "allegro.h"
#include "sprite.h"
#include "GalacticDefense.h"
#include "spritehandler.h"

// Print in correct format
void print_formated(const char* text, int x1, int x2, int y, int col, int bg) {
	
	// Buffer and initial variables
	char line[256];
	int end_position = strcspn(text, "\0");
	int current_y = y;
	int length = x2 - x1;
	
	// Check if we need to format
	if ((end_position * CHAR_PER_LENGTH) > length) {
		
		// Inital variables
		int start = 0;
		int end;
		int curr_space = 0;
		int next_space = 0;
		
		// Cycle through spaces and print on next line if doesn't fit
		while(next_space < end_position - 1) {
			next_space += strcspn(&text[curr_space], " ") + 1;
			if (((next_space - start) *  CHAR_PER_LENGTH) < length) {
				curr_space = next_space;
			}
			else {
				strncpy(line, &text[start], curr_space - start);
				strncpy(&line[curr_space - 1 - start], "\0", 1);
				textprintf_ex(screen, font, x1, y, col, bg, "%s", line);
				start = curr_space;
				y += 10;
			}
		}
		strncpy(line, &text[start], end_position - start);
		strncpy(&line[end_position - start], "\0", 1);
		textprintf_ex(screen, font, x1, y, col, bg, "%s", line);
	}
	else {
		textprintf_ex(screen, font, x1, y, col, bg, "%s", text);
	}
}

// Draw initial start screen instructions
void draw_startscreen() {
	
	textout_centre_ex(screen, font, "Galactic Defense", WIDTH/2, HEIGHT/4, WHITE, BLACK);
    textout_centre_ex(screen, font, "Press ENTER to Start!", WIDTH/2, HEIGHT/2, WHITE, BLACK);
    textout_centre_ex(screen, font, "INSTRUCTIONS:", WIDTH/2, HEIGHT/2 + 40, WHITE, BLACK);
    textout_ex(screen, font, "1. Use the arrow keys to navigate your space ship", WIDTH/8, HEIGHT/2 + 60, WHITE, BLACK);
    textout_ex(screen, font, "   Use the UP and DOWN key to accelerate and decelerate", WIDTH/8, HEIGHT/2 + 70, YELLOW, BLACK);
    textout_ex(screen, font, "   Use the LEFT and RIGHT key to TURN", WIDTH/8, HEIGHT/2 + 80, YELLOW, BLACK);
    textout_ex(screen, font, "2. Use the SPACE bar key to shoot", WIDTH/8, HEIGHT/2 + 90, WHITE, BLACK);
    print_formated("3. Use the 'g' key to activate a pulse that pushes all asteroids within a small area away", WIDTH/8, WIDTH - 10, HEIGHT/2 + 100, WHITE, BLACK);
    textout_ex(screen, font, "4. Press Esc to exit the game!", WIDTH/8, HEIGHT/2 + 120, WHITE, BLACK);
}

int main(void)
{
	//initialize Allegro
    allegro_init(); 
    
    //initialize the keyboard
    install_keyboard(); 
    
    //initalize timer
    install_timer();
    
    //initialize random seed
    srand(time(NULL));
    
    // Set color depth
    set_color_depth(desktop_color_depth());
    
    //Initialize Graphics
    int ret;
    ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    if (ret != 0) {
        allegro_message(allegro_error);
        return 0;
    }
    
    draw_startscreen();
    
    while(1) {
    	if (key[KEY_ENTER]) {
    		break;
    	}
    	else if (key[KEY_ESC]) {
    		allegro_exit();
    		exit(0);
    	}
	};
	
	// Clear Screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    
    BITMAP* image = load_bitmap(SPACESHIP_SPRITE, NULL);
    BITMAP* image2 = load_bitmap(BACKGROUND_SPRITE, NULL);
    if (!image || !image2) {
		allegro_message("Error loading background");
		return 1;
	}
    
    blit(image2, screen, 0, 0, 0, 0, WIDTH, HEIGHT);
    draw_sprite(screen, image, WIDTH/2, HEIGHT/2);
    
    rest(1000);
    
    while(1) {
    	if (key[KEY_ENTER]) {
    		break;
    	}
    	else if (key[KEY_ESC]) {
    		allegro_exit();
    		exit(0);
    	}
	};
    
    //end program
    allegro_exit();
    return 0;
}
END_OF_MAIN()
