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

void thrusters(int dir)
{
    //shift 0-degree orientation from right-face to up-face
	spaceship->moveAngle = spaceship->faceAngle - 90;
    //convert negative angle to wraparound
	if (spaceship->moveAngle < 0) spaceship->moveAngle = 359 + spaceship->moveAngle;

    //adjust velocity based on angle
    if (dir == 1) {
    	spaceship->velx += calcAngleMoveX(spaceship->moveAngle) * ACCELERATION;
		spaceship->vely += calcAngleMoveY(spaceship->moveAngle) * ACCELERATION;
    }
    else {
    	spaceship->velx -= calcAngleMoveX(spaceship->moveAngle) * ACCELERATION;
		spaceship->vely -= calcAngleMoveY(spaceship->moveAngle) * ACCELERATION;
    }
	
	//keep velocity down to a reasonable speed
	if (spaceship->velx > 4.0) {
		spaceship->velx = 4.0;
	}
	
	if (spaceship->velx < -4.0) {
		spaceship->velx = -4.0;
	}
	
	if (spaceship->vely > 4.0) {
		spaceship->vely = 4.0;
	}
	
	if (spaceship->vely < -4.0) {
		spaceship->vely = -4.0;
	}
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

void fireweapon()
{
    bullets->get(bullet_index)->x = spaceship->pointer_x - (bullets->get(bullet_index)->width / 2);
    bullets->get(bullet_index)->y = spaceship->pointer_y - (bullets->get(bullet_index)->height / 2);
    bullets->get(bullet_index)->faceAngle = spaceship->faceAngle;
    
    //shift 0-degree orientation from right-face to up-face
	bullets->get(bullet_index)->moveAngle = bullets->get(bullet_index)->faceAngle - 90;
	
    //convert negative angle to wraparound
	if (bullets->get(bullet_index)->moveAngle < 0) {
		bullets->get(bullet_index)->moveAngle = 359 + bullets->get(bullet_index)->moveAngle;
	}
    bullets->get(bullet_index)->velx = calcAngleMoveX(bullets->get(bullet_index)->moveAngle) * BULLETSPEED;
    bullets->get(bullet_index)->vely = calcAngleMoveY(bullets->get(bullet_index)->moveAngle) * BULLETSPEED;
	bullets->get(bullet_index)->alive = 1;
	
	bullet_index++;
	
	if (bullet_index > BULLET_CAP - 1) {
		bullet_index = 0;
	}
	bullet_cooldown = BULLETCOOLDOWN;
}

void updatebullet(int num)
{
    int x, y, tx, ty;
		
	//move the bullet
	bullets->get(num)->updatePosition();

    //move bullet
    bullets->get(num)->x += bullets->get(num)->velx;
    bullets->get(num)->y += bullets->get(num)->vely;
    x = bullets->get(num)->x;
    y = bullets->get(num)->y;

    //stay within the screen
    if (x < 6 || x > SCREEN_W-6 || y < 20 || y > SCREEN_H-6)
    {
        bullets->get(num)->alive = 0;
        return;
    }

    // Check if collide with asteroid
    
    // Else draw it
    rotate_sprite(buffer, bullets->get(num)->image, (int)bullets->get(num)->x, bullets->get(num)->y, 
		itofix((int)(bullets->get(num)->faceAngle / 0.7f / 2.0f)));
}

void updateasteroid(int num)
{
		
	// draw it
    rotate_sprite(buffer, asteroids->get(num)->image, (int)asteroids->get(num)->x, asteroids->get(num)->y, 
		itofix((int)(asteroids->get(num)->faceAngle / 0.7f / 2.0f)));
		
	//move the asteroid
	asteroids->get(num)->updatePosition();
		
	warpsprite(asteroids->get(num));
}

void update()
{
	//Loop variable
	int i;
	
	//Clear background
	blit(background, buffer, 0, 0, 0, 0, WIDTH, HEIGHT);
    
    //rotate sprite with adjust for Allegro's 16.16 fixed trig
    //(256 / 360 = 0.7), then divide by 2 radians
	rotate_sprite(buffer, spaceship->image, (int)spaceship->x, (int)spaceship->y, 
        itofix((int)(spaceship->faceAngle / 0.7f / 2.0f)));
        
    for (i = 0; i < ASTEROID_COUNT; i++) {
    	updateasteroid(i);
    }
        
    for (i = 0; i < BULLET_CAP; i++) {
    	if (bullets->get(i)->alive == 1) {
    		updatebullet(i);
		}
    }
        
    //move the spaceship
	spaceship->updatePosition();
	
	spaceship->pointer_x = spaceship->x + spaceship->width / 2 + calcAngleMoveX(spaceship->faceAngle - 90) * (spaceship->height / 2);
	spaceship->pointer_y = spaceship->y + spaceship->height / 2 + calcAngleMoveY(spaceship->faceAngle - 90) * (spaceship->height / 2);
	
    warpsprite(spaceship);
}

void getinput()
{
    //hit ESC to quit
    if (key[KEY_ESC])   gameover = 1;
    
    //ARROW KEYS AND SPACE BAR CONTROL
    if (key[KEY_UP]) {  
		thrusters(1);
	}
	
    if (key[KEY_DOWN]) {
		thrusters(-1);
	}
	
    if (key[KEY_LEFT]) {
		turnleft();
	}
	
    if (key[KEY_RIGHT]) {
		turnright();
	}
	
    if (key[KEY_SPACE]) {
    	if (bullet_cooldown == 0) {
    		fireweapon();
    	}
	}

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
	//Loop variable
	int i;	

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
	
    spaceship->width = spaceship->image->w;
    spaceship->height = spaceship->image->h;
    spaceship->xdelay = 0;
    spaceship->ydelay = 0;
    spaceship->x = SCREEN_W / 2 - spaceship->width/2;
    spaceship->y = SCREEN_H / 2 - spaceship->height/2;
	spaceship->moveAngle = 0;
	spaceship->faceAngle = 0;
	
	bullets = new spritehandler();
	
	for (i = 0; i < BULLET_CAP; i++) {
		bullets->create();
		bullets->get(i)->load(BULLET_SPRITE);
		bullets->get(i)->width = bullets->get(i)->image->w;
		bullets->get(i)->height = bullets->get(i)->image->h;
		bullets->get(i)->xdelay = 1;
		bullets->get(i)->ydelay = 1;
		bullets->get(i)->x = 0;
		bullets->get(i)->y = 0;
	}
	
	asteroids = new spritehandler();
	for (i = 0; i < SMALLASTEROID_COUNT; i++) {
		asteroids->create();
		asteroids->get(i)->load(SMALL_ASTEROID_SPRITE);
		asteroids->get(i)->width = asteroids->get(i)->image->w;
		asteroids->get(i)->height = asteroids->get(i)->image->h;
		asteroids->get(i)->xdelay = rand() % 3;
		asteroids->get(i)->ydelay = rand() % 3;
		if (rand() % 2 == 0) {
			asteroids->get(i)->x = rand() % (SCREEN_W / 2 - asteroids->get(i)->width - spaceship->width * 2);
		}
		else {
			asteroids->get(i)->x = rand() % (SCREEN_W / 2 + asteroids->get(i)->width + spaceship->width * 2);
		}
		
		if (rand() % 2 == 0) {
			asteroids->get(i)->y = rand() % (SCREEN_H / 2 - asteroids->get(i)->height - spaceship->height * 2);
		}
		else {
			asteroids->get(i)->y = rand() % (SCREEN_H / 2 + asteroids->get(i)->height + spaceship->height * 2);
		}
		asteroids->get(i)->faceAngle = rand() % 360;
		asteroids->get(i)->moveAngle = rand() % 360;
		asteroids->get(i)->velx = calcAngleMoveX(asteroids->get(i)->moveAngle) * ((rand() % (ASTEROID_SPEED - 1)) + 1);
		asteroids->get(i)->vely = calcAngleMoveY(asteroids->get(i)->moveAngle) * ((rand() % (ASTEROID_SPEED - 1)) + 1);
	}
	
	for (i = SMALLASTEROID_COUNT; i < ASTEROID_COUNT; i++) {
		asteroids->create();
		asteroids->get(i)->load(LARGE_ASTEROID_SPRITE);
		asteroids->get(i)->width = asteroids->get(i)->image->w;
		asteroids->get(i)->height = asteroids->get(i)->image->h;
		asteroids->get(i)->xdelay = rand() % 3;
		asteroids->get(i)->ydelay = rand() % 3;
		asteroids->get(i)->x = rand() % (SCREEN_W - asteroids->get(i)->width);
		asteroids->get(i)->y = rand() % (SCREEN_H - asteroids->get(i)->height);
		if (rand() % 2 == 0) {
			asteroids->get(i)->x = rand() % (SCREEN_W / 2 - asteroids->get(i)->width - spaceship->width * 2);
		}
		else {
			asteroids->get(i)->x = rand() % (SCREEN_W / 2 + asteroids->get(i)->width + spaceship->width * 2);
		}
		
		if (rand() % 2 == 0) {
			asteroids->get(i)->y = rand() % (SCREEN_H / 2 - asteroids->get(i)->height - spaceship->height * 2);
		}
		else {
			asteroids->get(i)->y = rand() % (SCREEN_H / 2 + asteroids->get(i)->height + spaceship->height * 2);
		}
		asteroids->get(i)->faceAngle = rand() % 360;
		asteroids->get(i)->moveAngle = rand() % 360;
		asteroids->get(i)->velx = calcAngleMoveX(asteroids->get(i)->moveAngle) * ((rand() % (ASTEROID_SPEED - 1)) + 1);
		asteroids->get(i)->vely = calcAngleMoveY(asteroids->get(i)->moveAngle) * ((rand() % (ASTEROID_SPEED - 1)) + 1);
	}
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
		if (bullet_cooldown > 0) {
			bullet_cooldown--;	
		}
	    rest(10);
    }
    
    //end program
    allegro_exit();
    return 0;
}
END_OF_MAIN()
