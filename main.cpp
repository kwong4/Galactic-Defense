#include <stdio.h>
#include "allegro.h"
#include "sprite.h"
#include "spritehandler.h"

#define BLACK makecol(0,0,0)
#define WHITE makecol(255,255,255)

#define NUM 20
#define WIDTH 800
#define HEIGHT 600
#define MODE GFX_AUTODETECT_WINDOWED

int main(void)
{
	//initialize Allegro
    allegro_init(); 
    
    //initialize the keyboard
    install_keyboard(); 
    
    //initialize random seed
    srand(time(NULL));
    
    //end program
    allegro_exit();
    return 0;
}
END_OF_MAIN()
