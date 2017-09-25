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

void instructions() {
	rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
	
	textout_centre_ex(screen, font, "GAME INFO:", WIDTH/2, HEIGHT/4, WHITE, BLACK);
	textout_ex(screen, font, "- You are tasked with the defense of the Galaxy.", WIDTH/8, HEIGHT/4 + 20, WHITE, BLACK);
	textout_ex(screen, font, "- Your goal is to destory as much asteroids before your ship needs repairs", WIDTH/8, HEIGHT/4 + 30, WHITE, BLACK);
	textout_ex(screen, font, "- You can take a total of 3 hits before your ship needs repairs", WIDTH/8, HEIGHT/4 + 40, WHITE, BLACK);
	textout_ex(screen, font, "- Small Asteroids take 1 hit to destory. But Big Asteroids require 3 hits to destory.", WIDTH/8, HEIGHT/4 + 50, WHITE, BLACK);
	textout_ex(screen, font, "- Easy gamemode will allow asteroids to pass by each other in predictable movement", WIDTH/8, HEIGHT/4 + 70, WHITE, BLACK);
	textout_ex(screen, font, "- Hard gamemode will make asteroids have unpredictable movements due to magnetic", WIDTH/8, HEIGHT/4 + 80, WHITE, BLACK);
	textout_ex(screen, font, "    properties of each asteroid", WIDTH/8, HEIGHT/4 + 90, WHITE, BLACK);
	
	textout_centre_ex(screen, font, "INSTRUCTIONS:", WIDTH/2, HEIGHT/2 + 40, WHITE, BLACK);
    textout_ex(screen, font, "1. Use the arrow keys to navigate your space ship", WIDTH/8, HEIGHT/2 + 60, WHITE, BLACK);
    textout_ex(screen, font, "   Use the UP and DOWN key to accelerate and decelerate", WIDTH/8, HEIGHT/2 + 70, YELLOW, BLACK);
    textout_ex(screen, font, "   Use the LEFT and RIGHT key to TURN", WIDTH/8, HEIGHT/2 + 80, YELLOW, BLACK);
    textout_ex(screen, font, "2. Use the SPACE bar key to shoot", WIDTH/8, HEIGHT/2 + 90, WHITE, BLACK);
    print_formated("3. Use the 'g' key to activate a pulse that pushes all asteroids within a small area away", WIDTH/8, WIDTH - 10, HEIGHT/2 + 100, WHITE, BLACK);
    textout_ex(screen, font, "4. Press Esc to exit the game!", WIDTH/8, HEIGHT/2 + 120, WHITE, BLACK);
    
    textout_centre_ex(screen, font, "Press ENTER to return", WIDTH/2, HEIGHT/2 + 200, WHITE, BLACK);
    
    rest(250);
    
    while(1) {
    	if (key[KEY_ENTER]) {
			break;
		}
		
		if (key[KEY_ESC]) {
			allegro_exit();
			exit(0);	
		}
    }
}

int getmenuinput() {
	
	// If user quits game
	if (key[KEY_ESC]) {
		allegro_exit();
		exit(0);
	}
	
	// Move cursor for selection
	if (key[KEY_DOWN] && selection != max_selection) {
		rectfill(screen, WIDTH/3 + 1, HEIGHT/2 + selection * 15 + 81, WIDTH/3 + 9, HEIGHT/2 + selection * 15 + 89, BLACK);
		selection++;
		rectfill(screen, WIDTH/3 + 1, HEIGHT/2 +  selection * 15 + 81, WIDTH/3 + 9, HEIGHT/2 + selection * 15 + 89, WHITE);
	}
	else if (key[KEY_UP] && selection != 0) {
		rectfill(screen, WIDTH/3 + 1, HEIGHT/2 + selection * 15 + 81, WIDTH/3 + 9, HEIGHT/2 +  selection * 15 + 89, BLACK);
		selection--;
		rectfill(screen, WIDTH/3 + 1, HEIGHT/2 + selection * 15 + 81, WIDTH/3 + 9, HEIGHT/2 + selection * 15 + 89, WHITE);
	}
	else if (key[KEY_ENTER]) {
		return -1;
	}
}

void welcome_screen() {
	blit(title, screen, 0, 0, 150, 50, title->w, title->h);
	
    textout_centre_ex(screen, font, "Press use your ARROW KEYS and ENTER to select an option!", WIDTH/2, HEIGHT/2 + 40, WHITE, BLACK);
    rect(screen, WIDTH/3, HEIGHT/2 + 80 , WIDTH/3 + 10, HEIGHT/2 + 90, WHITE);
    textout_ex(screen, font, "Start!", WIDTH/3 + 25, HEIGHT/2 + 82, WHITE, BLACK);
    rect(screen, WIDTH/3, HEIGHT/2 + 95, WIDTH/3 + 10, HEIGHT/2 + 105, WHITE);
    textout_ex(screen, font, "Instructions", WIDTH/3 + 25, HEIGHT/2 + 97, WHITE, BLACK);
    rect(screen, WIDTH/3, HEIGHT/2 + 110, WIDTH/3 + 10, HEIGHT/2 + 120, WHITE);
    textout_ex(screen, font, "Gamemode:", WIDTH/3 + 25, HEIGHT/2 + 112, WHITE, BLACK);
    selection = 0;
    rectfill(screen, WIDTH/3 + 1, HEIGHT/2 + 81, WIDTH/3 + 9, HEIGHT/2 + 89, WHITE);
    
    if (hardmode == 0) {
    	textout_ex(screen, font, "Easy", WIDTH/3 + 100, HEIGHT/2 + 112, YELLOW, BLACK);
    }
    else {
    	textout_ex(screen, font, "Hard", WIDTH/3 + 100, HEIGHT/2 + 112, YELLOW, BLACK);
    }	
}

// Draw initial start screen instructions
void draw_startscreen() {
    
    welcome_screen();
    
    while (1) {
    	if (getmenuinput() == -1) {
    		if (selection == 0) {
    			break;
    		}
    		else if (selection == 1) {
    			instructions();
    			rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    			welcome_screen();
    		}
    		else {
    			hardmode = (hardmode + 1) % 2;
    			if (hardmode == 0) {
		    		textout_ex(screen, font, "Easy", WIDTH/3 + 100, HEIGHT/2 + 112, YELLOW, BLACK);
			    }
			    else {
			    	textout_ex(screen, font, "Hard", WIDTH/3 + 100, HEIGHT/2 + 112, YELLOW, BLACK);
			    }
    		}
    	}
	    rest(100);
    }
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

void restart_asteroid(int num) {
	
	int collide;
	int i;
	
	while (1) {
			collide = 0;
			
			if (rand() % 2 == 0) {
				if (rand() % 2 == 0) {
					asteroids->get(num)->x = 0;
					asteroids->get(num)->y = rand() % (SCREEN_H - asteroids->get(num)->height);
				}
				else {
					asteroids->get(num)->x = SCREEN_W - asteroids->get(num)->width;
					asteroids->get(num)->y = rand() % (SCREEN_H - asteroids->get(num)->height);
				}
			}
			else {
				if (rand() % 2 == 0) {
					asteroids->get(num)->x = rand() % (SCREEN_W - asteroids->get(num)->width);
					asteroids->get(num)->y = 0;
				}
				else {
					asteroids->get(num)->x = rand() % (SCREEN_W - asteroids->get(num)->width);
					asteroids->get(num)->y = SCREEN_H - asteroids->get(num)->height;
				}
			}
			
			for (i = 0; i < ASTEROID_COUNT; i++) {
				if (i != num && asteroids->get(i)->collided(asteroids->get(num))) {
        			collide = 1;
					break;	
        		}
			}
			
			if (collide == 0) {
				break;
			}
		}
		
	asteroids->get(num)->velx = (rand() % 12) - 6;
	asteroids->get(num)->vely = (rand() % 12) - 6;
	asteroids->get(num)->alive = 1;	
	
	if (num >= SMALLASTEROID_COUNT) {
		asteroids->get(num)->health = 3;
	}
	else {
		asteroids->get(num)->health = 1;
	}
	
}

void checkcollisions_asteroid(int num)
{
    int cx1,cy1,cx2,cy2;
    int i;

    for (i=0; i<ASTEROID_COUNT; i++)
    {
        if (i != num && asteroids->get(i)->collided(asteroids->get(num)))
        {
        	
            //calculate center of primary sprite
            cx1 = (int)asteroids->get(i)->x + asteroids->get(i)->width / 2;
            cy1 = (int)asteroids->get(i)->y + asteroids->get(i)->height / 2;
            
            //calculate center of secondary sprite
            cx2 = (int)asteroids->get(i)->x + asteroids->get(i)->width / 2;
            cy2 = (int)asteroids->get(i)->y + asteroids->get(i)->height / 2;
            
            //figure out which way the sprites collided
            if (cx1 <= cx2)
            {
                asteroids->get(i)->velx = -1 * (rand() % 6) - 1;
                asteroids->get(num)->velx = rand() % 6 + 1;
                if (cy1 <= cy2)
                {
                    asteroids->get(i)->vely = -1 * (rand() % 6) - 1;
                    asteroids->get(num)->vely = rand() % 6 + 1;
                }
                else
                {
                    asteroids->get(i)->vely = rand() % 6 + 1;
                    asteroids->get(num)->vely = -1 * (rand() % 6) - 1;
                }
            }
            else
            {
                //cx1 is > cx2
                asteroids->get(i)->velx = rand() % 6 + 1;
                asteroids->get(num)->velx = -1 * (rand() % 6) - 1;
                if (cy1 <= cy2)
                {
                    asteroids->get(i)->vely = rand() % 6 + 1;
                    asteroids->get(num)->vely = -1 * (rand() % 6) - 1;
                }
                else
                {
                    asteroids->get(i)->vely = -1 * (rand() % 6) - 1;
                    asteroids->get(num)->vely = rand() % 6 + 1;
                }
            }
        }
    }
}

void checkcollisions_bullet(int num)
{
    int i;

    for (i=0; i<ASTEROID_COUNT; i++)
    {
        if (bullets->get(num)->collided(asteroids->get(i)))
        {
        	asteroids->get(i)->health--;
        	
        	if (asteroids->get(i)->health == 0) {
        		draw_sprite(buffer, explosion, bullets->get(num)->x, bullets->get(num)->y);
        		score += 100;
        		asteroids->get(i)->alive = 0;
        	}
        	
        	bullets->get(num)->alive = 0;
        	break;
        }
    }
}

void checkcollisions_ship() {
	
	int i;

    for (i=0; i<ASTEROID_COUNT; i++)
    {
        if (spaceship->collided(asteroids->get(i)))
        {
        	spaceship->health -= 1;
        	asteroids->get(i)->alive = 0;
        	if (spaceship->health == 0) {
        		spaceship->alive = 0;
        		gameover = 1;
        	}
        }
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
	
    x = bullets->get(num)->x;
    y = bullets->get(num)->y;

    //stay within the screen
    if (x < 6 || x > SCREEN_W-6 || y < 20 || y > SCREEN_H-6)
    {
        bullets->get(num)->alive = 0;
        return;
    }

    // Check if collide with asteroid
    checkcollisions_bullet(num);
    
    // Else draw it
    if (bullets->get(num)->alive) {
    	rotate_sprite(buffer, bullets->get(num)->image, (int)bullets->get(num)->x, bullets->get(num)->y, 
		itofix((int)(bullets->get(num)->faceAngle / 0.7f / 2.0f)));
    }
}

void updateasteroid(int num)
{
		
    //rotate_sprite(buffer, asteroids->get(num)->image, (int)asteroids->get(num)->x, asteroids->get(num)->y, 
	//	itofix((int)(asteroids->get(num)->faceAngle / 0.7f / 2.0f)));
		
	// draw it
	
	if (asteroids->get(num)->health == 3 || num < SMALLASTEROID_COUNT) {
		draw_sprite(buffer, asteroids->get(num)->image, (int)asteroids->get(num)->x, asteroids->get(num)->y);
	}
	else if (asteroids->get(num)->health == 2) {
		draw_sprite(buffer, asteroids->get(num)->image2, (int)asteroids->get(num)->x, asteroids->get(num)->y);
	}
	else {
		draw_sprite(buffer, asteroids->get(num)->image3, (int)asteroids->get(num)->x, asteroids->get(num)->y);
	}
		
	//move the asteroid
	asteroids->get(num)->updatePosition();
		
	if (hardmode == 1) {
		checkcollisions_asteroid(num);
	}
	
	warpsprite(asteroids->get(num));
}

void updatehealth() {
	rect(buffer, 60, 2, 90, 14, RED); 
	rect(buffer, 90, 2, 120, 14, RED); 
	rect(buffer, 120, 2, 150, 14, RED); 
	
	if (spaceship->health > 0) {
		rectfill(buffer, 62, 4, 88, 12, RED);
	}
	
	if (spaceship->health > 1) {
		rectfill(buffer, 92, 4, 118, 12, RED);	
	}
	
	if (spaceship->health > 2) {
		rectfill(buffer, 122, 4, 148, 12, RED);
	}
}

void update()
{
	//Loop variable
	int i;
	
	//Clear background
	blit(background, buffer, 0, 0, 0, 0, WIDTH, HEIGHT);
	
	for (i = 0; i < BULLET_CAP; i++) {
    	if (bullets->get(i)->alive == 1) {
    		updatebullet(i);
		}
    }
        
    for (i = 0; i < ASTEROID_COUNT; i++) {
    	if (asteroids->get(i)->alive == 1) {
    		updateasteroid(i);
    	}
    	else {
    		restart_asteroid(i);
    	}
    }
    
    rectfill(buffer, 0, 0, WIDTH, 16, BLACK); 
    
    textout_ex(buffer, font, "Health:", 1, 4, WHITE, BLACK);
    textprintf_centre_ex(buffer, font, WIDTH/2, 4, WHITE, BLACK, "Score: %i", score);
    
    updatehealth();
        
    checkcollisions_ship();
        
    //move the spaceship
	spaceship->updatePosition();
	
	spaceship->pointer_x = spaceship->x + spaceship->width / 2 + calcAngleMoveX(spaceship->faceAngle - 90) * (spaceship->height / 2);
	spaceship->pointer_y = spaceship->y + spaceship->height / 2 + calcAngleMoveY(spaceship->faceAngle - 90) * (spaceship->height / 2);
	
	//rotate sprite with adjust for Allegro's 16.16 fixed trig
    //(256 / 360 = 0.7), then divide by 2 radians
	rotate_sprite(buffer, spaceship->image, (int)spaceship->x, (int)spaceship->y, 
        itofix((int)(spaceship->faceAngle / 0.7f / 2.0f)));
	
    warpsprite(spaceship);
}

void getinput()
{
    //hit ESC to quit
    if (key[KEY_ESC]) {
		gameover = 1;
	}
	
	if (key[KEY_LCONTROL] && key[KEY_H]) {
		instructions();
	}
    
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
	
	//load background buffer
	title = load_bitmap(GALATICDEFENSE, NULL);
	if (!title) {
		allegro_message("Error loading sprites");
		return;
	}
	
	draw_startscreen();
}

void setupgame()
{
	//Loop variable
	int i;	
	int j;
	int collide;

	//Create a back buffer
	buffer = create_bitmap(WIDTH,HEIGHT);
	
	//load background buffer
	background = load_bitmap(BACKGROUND_SPRITE, NULL);
	
	//load explosion buffer
	explosion = load_bitmap(EXPLOSION, NULL);
	
	//Create a spaceship sprite
	spaceship = new sprite();
	if (!spaceship->load(SPACESHIP_SPRITE) || !background || !explosion) {
		allegro_message("Error loading sprites");
		return;
	}
	
    spaceship->width = spaceship->image->w;
    spaceship->height = spaceship->image->h;
    spaceship->health = 3;
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
		bullets->get(i)->xdelay = 0;
		bullets->get(i)->ydelay = 0;
		bullets->get(i)->x = 0;
		bullets->get(i)->y = 0;
	}
	
	asteroids = new spritehandler();
	for (i = 0; i < SMALLASTEROID_COUNT; i++) {
		asteroids->create();
		asteroids->get(i)->load(SMALL_ASTEROID_SPRITE);
		asteroids->get(i)->health = 1;
		asteroids->get(i)->width = asteroids->get(i)->image->w;
		asteroids->get(i)->height = asteroids->get(i)->image->h;
		asteroids->get(i)->xdelay = 1;
		asteroids->get(i)->ydelay = 1;
		asteroids->get(i)->alive = 1;
		while (1) {
			collide = 0;
			
			if (rand() % 2 == 0) {
				asteroids->get(i)->x = rand() % ((SCREEN_W / 2) - (asteroids->get(i)->width + spaceship->width));
			}
			else {
				asteroids->get(i)->x = rand() % ((SCREEN_W / 2) - (asteroids->get(i)->width + spaceship->width)) + ((SCREEN_W / 2) + (asteroids->get(i)->width + spaceship->width));
			}
			
			if (rand() % 2 == 0) {
				asteroids->get(i)->y = rand() % (SCREEN_H / 2) - (asteroids->get(i)->height + spaceship->height);
			}
			else {
				asteroids->get(i)->y = rand() % ((SCREEN_H / 2) - (asteroids->get(i)->height + spaceship->height)) + ((SCREEN_H / 2) + (asteroids->get(i)->height + spaceship->height));
			}
			
			for (j = 0; j < i; j++) {
				if (asteroids->get(j)->collided(asteroids->get(i))) {
        			collide = 1;
					break;	
        		}
			}
			
			if (collide == 0) {
				break;
			}
		}
		
		asteroids->get(i)->velx = (rand() % 12) - 6;
		asteroids->get(i)->vely = (rand() % 12) - 6;
	}
	
	for (i = SMALLASTEROID_COUNT; i < ASTEROID_COUNT; i++) {
		asteroids->create();
		asteroids->get(i)->load(LARGE_ASTEROID_SPRITE);
		asteroids->get(i)->load2(LARGE_ASTEROID_SPRITE2);
		asteroids->get(i)->load3(LARGE_ASTEROID_SPRITE3);
		asteroids->get(i)->health = 3;
		asteroids->get(i)->width = asteroids->get(i)->image->w;
		asteroids->get(i)->height = asteroids->get(i)->image->h;
		asteroids->get(i)->xdelay = 1;
		asteroids->get(i)->ydelay = 1;
		asteroids->get(i)->alive = 1;
		while (1) {
			collide = 0;
			
			if (rand() % 2 == 0) {
				asteroids->get(i)->x = rand() % ((SCREEN_W / 2) - (asteroids->get(i)->width + spaceship->width));
			}
			else {
				asteroids->get(i)->x = rand() % ((SCREEN_W / 2) - (asteroids->get(i)->width + spaceship->width)) + ((SCREEN_W / 2) + (asteroids->get(i)->width + spaceship->width));
			}
			
			if (rand() % 2 == 0) {
				asteroids->get(i)->y = rand() % (SCREEN_H / 2) - (asteroids->get(i)->height + spaceship->height);
			}
			else {
				asteroids->get(i)->y = rand() % ((SCREEN_H / 2) - (asteroids->get(i)->height + spaceship->height)) + ((SCREEN_H / 2) + (asteroids->get(i)->height + spaceship->height));
			}
			
			for (j = 0; j < i; j++) {
				if (asteroids->get(j)->collided(asteroids->get(i))) {
	    			collide = 1;
					break;	
	    		}
			}
			
			if (collide == 0) {
				break;
			}
		}
		
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
    
    // Clear Screen
    rectfill(screen, 0, 0, WIDTH, HEIGHT, BLACK);
    
    // Print final score
    textprintf_centre_ex(screen, font, WIDTH/2, HEIGHT/2, WHITE, BLACK, "Final User Score: %i", score);
    textout_centre_ex(screen, font, "Please Press ESC to QUIT!", WIDTH/2, HEIGHT/2 + 20, WHITE, BLACK);
    
    rest(250);
    
    // Wait until user exits game
    while(!key[KEY_ESC]) {
	};
    
    //end program
    allegro_exit();
    return 0;
}
END_OF_MAIN()
