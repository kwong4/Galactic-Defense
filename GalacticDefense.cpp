#include <stdio.h>
#include "allegro.h"
#include "math.h"
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

//calculate X movement value based on direction angle
double calcAngleMoveX(int angle) {
    return (double) cos(angle * PI / 180);
}

//calculate Y movement value based on direction angle
double calcAngleMoveY(int angle) {
    return (double) sin(angle * PI / 180);
}

void erasesprite(BITMAP *dest, sprite *spr)
{
    //erase the sprite using BLACK color fill
    rectfill(dest, (int)spr->x, (int)spr->y, (int)spr->x + spr->width, 
        (int)spr->y + spr->height, BLACK);
}

void warpsprite(sprite *spr)
{
    //simple screen warping behavior
    int w = spr->width;
    int h = spr->height;
    
    if (spr->x < 0-w)
    {
        spr->x = SCREEN_W;
    }

    else if (spr->x > SCREEN_W)
    {
        spr->x = 0-w;
    }

    if (spr->y < 0-h)
    {
        spr->y = SCREEN_H;
    }

    else if (spr->y > SCREEN_H)
    {
        spr->y = 0-h;
    }

}

void forward()
{
    //shift 0-degree orientation from right-face to up-face
	spaceship->moveAngle = spaceship->faceAngle - 90;
    //convert negative angle to wraparound
	if (spaceship->moveAngle < 0) spaceship->moveAngle = 359 + spaceship->moveAngle;

    //adjust velocity based on angle
	spaceship->velx += calcAngleMoveX(spaceship->moveAngle) * ACCELERATION;
	spaceship->vely += calcAngleMoveY(spaceship->moveAngle) * ACCELERATION;
	
	//keep velocity down to a reasonable speed
	if (spaceship->velx > 4.0) spaceship->velx = 4.0;
	if (spaceship->velx < -4.0) spaceship->velx = -4.0;
	if (spaceship->vely > 4.0) spaceship->vely = 4.0;
	if (spaceship->vely < -4.0) spaceship->vely = -4.0;
}

void backward()
{
    //shift 0-degree orientation from right-face to up-face
	spaceship->moveAngle = spaceship->faceAngle - 90;
    //convert negative angle to wraparound
	if (spaceship->moveAngle < 0) spaceship->moveAngle = 359 + spaceship->moveAngle;

    //adjust velocity based on angle
	spaceship->velx -= calcAngleMoveX(spaceship->moveAngle) * ACCELERATION;
	spaceship->vely -= calcAngleMoveY(spaceship->moveAngle) * ACCELERATION;
	
	//keep velocity down to a reasonable speed
	if (spaceship->velx > 4.0) spaceship->velx = 4.0;
	if (spaceship->velx < -4.0) spaceship->velx = -4.0;
	if (spaceship->vely > 4.0) spaceship->vely = 4.0;
	if (spaceship->vely < -4.0) spaceship->vely = -4.0;
}

void turnleft()
{
    spaceship->faceAngle -= STEP;
	if (spaceship->faceAngle < 0) {
		spaceship->faceAngle = 359;
	}	
}

void turnright()
{
    spaceship->faceAngle += STEP;
	if (spaceship->faceAngle > 359) {
		spaceship->faceAngle = 0;
	}
}

void update()
{
	//Clear background
	blit(background, buffer, 0, 0, 0, 0, WIDTH, HEIGHT);
    
    //rotate sprite with adjust for Allegro's 16.16 fixed trig
    //(256 / 360 = 0.7), then divide by 2 radians
	rotate_sprite(buffer, spaceship->image, (int)spaceship->x, (int)spaceship->y, 
        itofix((int)(spaceship->faceAngle / 0.7f / 2.0f)));
        
    //move the spaceship
	spaceship->updatePosition();
    warpsprite(spaceship);
}

void getinput()
{
    //hit ESC to quit
    if (key[KEY_ESC])   gameover = 1;
    
    //ARROW KEYS AND SPACE BAR CONTROL
    if (key[KEY_UP]) {  
		forward();
	}
	
    if (key[KEY_DOWN]) {
		backward();
	}
	
    if (key[KEY_LEFT]) {
		turnleft();
	}
	
    if (key[KEY_RIGHT]) {
		turnright();
	}
    //if (key[KEY_SPACE]) fireweapon(0);

    //short delay after keypress        
    rest(20);
}

void setupscreen()
{
    int ret;

    //set video mode    
    set_color_depth(desktop_color_depth());
    ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    if (ret != 0) {
        allegro_message(allegro_error);
        return;
    }
   
   	draw_startscreen();
}

void setupgame()
{
	//Create a back buffer
	buffer = create_bitmap(WIDTH,HEIGHT);
	
	//load background buffer
	background = load_bitmap(BACKGROUND_SPRITE, NULL);
	
	//Create a spaceship sprite
	spaceship = new sprite();
	if (!spaceship->load(SPACESHIP_SPRITE) || !background) {
		allegro_message("Error loading sprites");
		return;
	}
	
    spaceship->width = 40;
    spaceship->height = 50;
    spaceship->xdelay = 0;
    spaceship->ydelay = 0;
    spaceship->x = SCREEN_W / 2 - spaceship->width/2;
    spaceship->y = SCREEN_H / 2 - spaceship->height/2;
	spaceship->moveAngle = 0;
	spaceship->faceAngle = 0;
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
    
    setupscreen();
    
    
    while(1) {
    	if (key[KEY_ENTER]) {
    		break;
    	}
    	else if (key[KEY_ESC]) {
    		allegro_exit();
    		exit(0);
    	}
	};
	
	setupgame();
	
	// Clear Screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    
    while(!gameover) {
    	
    	//refresh the screen
	    acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, WIDTH, HEIGHT);
	    release_screen();
	    
	    //check for keypresses
        if (keypressed()) {
            getinput();
		}
		
		update();
	    rest(10);
    }
    
    //end program
    allegro_exit();
    return 0;
}
END_OF_MAIN()
