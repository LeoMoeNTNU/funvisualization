#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <SDL.h>
#include "./constants.h"

#define RAND0255 rand()%256

int randint() { return (rand() % COLORCHANGE) - COLORCHANGE / 2 ;}

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
int game_is_running = false;
int last_frame_time = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;


int clamp(int number, int min, int max)
{
    if(number<min)
        return min;

    if(number>max)
        return max;

    return number;
}

typedef struct Color
{
    char r;
    char g;
    char b;
}Color_t;



unsigned char colors[3*RECTANGLES*RECTANGLES]={0};
Color_t colors2[RECTANGLES*RECTANGLES]={0};
char basecolor=255/2;
static SDL_Rect ballcoords={40,40,20,20};


char randomint(){

}

///////////////////////////////////////////////////////////////////////////////
// Declare two game objects for the ball and the paddle
///////////////////////////////////////////////////////////////////////////////
struct game_object {
    int x;
    int y;
    int width;
    int height;
    int vel_x;
    int vel_y;
} ball, paddle;
//I dont know what the paddle is yet...

//This one only gives the color to the first one and not to the next two. Other two are gotten by ++
char* getTile(int x, int y){
    return colors +3 * (x + y * RECTANGLES);
}//no idea if the function should be static or not. 



///////////////////////////////////////////////////////////////////////////////
// Function to initialize our SDL window
///////////////////////////////////////////////////////////////////////////////
int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow(
        "A simple game loop using C & SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Function to poll SDL events and process keyboard input
///////////////////////////////////////////////////////////////////////////////
void process_input(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    game_is_running = false;
                }
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Setup function that runs once at the beginning of our program
///////////////////////////////////////////////////////////////////////////////
void setup(void) {
    // Initialize the ball object moving down at a constant velocity
    ball.x = 10;
    ball.y = 20;
    ball.width = 20;
    ball.height = 20;
    ball.vel_x = 180;
    ball.vel_y = 140;

    unsigned char a=1;
    while(a!=0){
        printf("%d\n",a++);
    }
    printf("%d\n",RECTANGLES);
    printf("%d\n",WIDTHHEIGHT);
    for(int i=0;i<10;i++){
        printf("%d\n",rand()%15);

    }





}

void buildcolors(){

    //setting first rgb
    colors[0]=basecolor;
    colors[1]=basecolor;
    colors[2]=basecolor;
    /*
    *bp=basecolor;
    *(bp+1)=basecolor;
    *(bp+2)=basecolor;
    
    
    */


    char change;//used in the for loop, no reason to redeclare. 

    char* bp=colors;

    //go all the way to the right. 
    for(int x=3;x<3*RECTANGLES;x++){
        //think about this access pattern a bit to be sure it is correct. 
        //I want to imitate it going downwards. 

        change=randint();//this gets generated. 
        *(bp+x)=*(bp+x-3)+change;
        //colors[x]=(colors[x-3]+RAND0255)/2;
        //this is legal for some dumb reason. 
    }

    //this goes down...
    for(int x=1;x<RECTANGLES;x++){

        //for doing it for not only one but all 3. 
        for(int coloroffset=0;coloroffset<3;coloroffset++){
            char *colorpointer=bp+coloroffset;
            change = randint();

            //*(colorpointer+3*RECTANGLES*x)=*(colorpointer+3*RECTANGLES*(x-1))+change;//no idea that this worked
            colorpointer[3 * RECTANGLES * x] = colorpointer[3 * RECTANGLES * (x - 1)] + change; // no idea that this worked
            //colorpointer[3*RECTANGLES*x]=(colorpointer[3*RECTANGLES*(x-1)]+RAND0255)/2;
        }   
    }
    //TODO: all the rest of the colors. 
    char* leftcp;
    char* uppercp;
    char* upperleftcp;
    char* currp;
    for(int x=1;x<RECTANGLES;x++){
        for(int y=1;y<RECTANGLES;y++){
            change=randint();
            currp=colors+3*(x+y*RECTANGLES);
            leftcp=currp-3;
            uppercp=currp-3*RECTANGLES;
            upperleftcp=currp-3*(RECTANGLES+1);
            for(int i=0;i<3;i++){
                change = randint();
                *(currp+i)=(*(leftcp+i)+*(uppercp+i)+*(upperleftcp+i))/3+change;
                //*(currp + i) = (*(leftcp + i) + *(uppercp + i)+RAND0255) / 3;
            }
        }
    }



}
//not made yet...
void displaycolors(){

    for(int row=0;row<RECTANGLES;row++){
        for(int col=0;col<RECTANGLES;col++){
            char *cp=colors+3*(row+col*RECTANGLES);//same as just adding...
            SDL_SetRenderDrawColor(renderer, *cp, *(cp+1), *(cp+2), 255);
            SDL_Rect fornow = {row * WIDTHHEIGHT, col * WIDTHHEIGHT, WIDTHHEIGHT, WIDTHHEIGHT};
            // ballcoords={x*WIDTHHEIGHT,y*WIDTHHEIGHT,WIDTHHEIGHT,WIDTHHEIGHT};
            SDL_RenderFillRect(renderer, &fornow);
        }
    }

    SDL_RenderPresent(renderer);

    /* These are for future use. 
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ballcoords);
    */
  
}


///////////////////////////////////////////////////////////////////////////////
// Update function with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
    // Get delta_time factor converted to seconds to be used to update objects
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

    // Store the milliseconds of the current frame to be used in the next one
    last_frame_time = SDL_GetTicks();

    // Move ball as a function of delta time
    ball.x += ball.vel_x * delta_time;
    ball.y += ball.vel_y * delta_time;

    // Check for ball collision with the window borders
    if (ball.x < 0) {
        ball.x = 0;
        ball.vel_x = -ball.vel_x;
    }
    if (ball.x + ball.height > WINDOW_WIDTH) {
        ball.x = WINDOW_WIDTH - ball.width;
        ball.vel_x = -ball.vel_x;
    }
    if (ball.y < 0) {
        ball.y = 0;
        ball.vel_y = -ball.vel_y;
    }
    if (ball.y + ball.height > WINDOW_HEIGHT) {
        ball.y = WINDOW_HEIGHT - ball.height;
        ball.vel_y = -ball.vel_y;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw game objects in the SDL window
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw a rectangle for the ball object
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height
    };
    //I dont like this construction. Should be static.  
    //I could make lots of this static. 


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderFillRect(renderer, &ball_rect);

    SDL_RenderFillRect(renderer,&ballcoords);

    SDL_RenderPresent(renderer);
}

///////////////////////////////////////////////////////////////////////////////
// Function to destroy SDL window and renderer
///////////////////////////////////////////////////////////////////////////////
void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* args[]) {
    srand(time(NULL));
    game_is_running = initialize_window();

    //setup(); not using it yet. 
    buildcolors();
    displaycolors();

    while (game_is_running) {
        process_input(); 
        //update(); Take this one back to get the ball moving. 
        //render(); this one may fuck with my shit but is nice. 
    }

    destroy_window();

    return 0;
}
