#include <SDL/SDL.h>

#define WIDTH 800
#define HEIGHT 800
#define BPP 32

int MAX_ITERATION = 100;
long double CENTERX = -0.75;
long double CENTERY = 0;
long double SIZE = 1.75;
int AUTO_DEZOOM = 0;
int AUTO_ZOOM = 0;
int PIXEL_SIZE = 1;





void setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
long double map(long double value, long double istart, long double istop, long double ostart, long double ostop);


int main(int argc, char *argv[])
{
    SDL_Surface *ecran = NULL;
    int i = 0;

    SDL_Init(SDL_INIT_VIDEO);

    ecran = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_HWSURFACE);

    
    SDL_WM_SetCaption("Mon dégradé en SDL !", NULL);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    int cont = 1;
    while(cont){

        // ############### INPUTS
        int wait = 1;
        SDL_Event event;
        while (wait)
        {
            while (SDL_PollEvent(&event)) {
                int posx, posy;
                switch (event.type) {
                    // exit if the window is closed
                    case SDL_QUIT:
                        wait = 0;
                        cont = 0;
                        break;
                        // check for keypresses
                    case SDL_MOUSEBUTTONDOWN:
                        SDL_GetMouseState(&posx, &posy);
                        CENTERX = map(posx, 0, WIDTH, CENTERX - SIZE, CENTERX + SIZE);
                        CENTERY = map(posy, 0, WIDTH, CENTERY - SIZE, CENTERY + SIZE);
                        break;
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym){
                            case SDLK_k:
                                wait = 0;
                                break;
                            case SDLK_a:
                                AUTO_ZOOM = ! AUTO_ZOOM;
                                AUTO_DEZOOM = 0;
                                break;
                            case SDLK_d:
                                AUTO_DEZOOM = ! AUTO_DEZOOM;
                                AUTO_ZOOM = 0;
                                break;
                            case SDLK_p:
                                PIXEL_SIZE += 1;
                                break;
                            case SDLK_o:
                                PIXEL_SIZE -= 1;
                                if(PIXEL_SIZE <= 0){
                                    PIXEL_SIZE = 1;
                                }
                                break;
                            case SDLK_m:
                                MAX_ITERATION += 1;
                                break;
                            case SDLK_l:
                                MAX_ITERATION -= 1;
                                if(MAX_ITERATION <= 0){
                                    MAX_ITERATION = 1;
                                }
                                break;
                            case SDLK_h:
                                printf("CENTERX : %Lf\nCENTERY : %Lf\nSIZE : %Lf\nMAX_ITERATION : %d\n\n", CENTERX, CENTERY, SIZE, MAX_ITERATION);
                                break;
                            default:
                                break;
                            
                        }
                        break;
                    default:
                        break;
                }
            }
            if(AUTO_ZOOM){
                wait = 0;
            } else if(AUTO_DEZOOM){
                wait = 0;
            }
        }



        if(AUTO_DEZOOM){
            SIZE *= 2;
        } else if (AUTO_ZOOM){
            SIZE /= 2;
        }

        // ############### CODE
        SDL_LockSurface(ecran);
        long double x0, y0, x, y, xx, yy, xtemp;
        int iteration;
        for(int px = 0; px < WIDTH; px += PIXEL_SIZE){
            for(int py = 0; py < HEIGHT; py += PIXEL_SIZE){
                x0 = map(px, 0, WIDTH, CENTERX - SIZE, CENTERX + SIZE);
                y0 = map(py, 0, HEIGHT, CENTERY - SIZE, CENTERY + SIZE);
                x = 0;
                y = 0;
                iteration = 0;
                xx = x*x;
                yy = y*y;
                while((xx + yy <= 4) && (iteration < MAX_ITERATION)){
                    xtemp = xx - yy + x0;
                    y = 2*x*y + y0;
                    x = xtemp;
                    iteration += 1;
                    xx = x*x;
                    yy = y*y;
                }
                long double c = map(iteration, 0, MAX_ITERATION, 0, 255);
                for(int i = 0; i < PIXEL_SIZE; i++){
                    if(px + i >= WIDTH){
                        break;
                    }
                    for(int j = 0; j < PIXEL_SIZE; j++){
                        if(py + j >= HEIGHT){
                            break;
                        }   
                        setPixel(ecran, px+i, py+j, SDL_MapRGB(ecran->format, c,c,c));
                    }
                }
            }
        }
        
        //setPixel(ecran, ecran->w / 2, ecran->h / 2, SDL_MapRGB(ecran->format, 255, 0, 0));
        SDL_UnlockSurface(ecran);

        SDL_Flip(ecran);

    }
    SDL_Quit();

    return EXIT_SUCCESS;
}



/*

 sudo apt-get install libsdl2-dev
 sudo apt-get install libsdl2-2.0
 sudo apt-get install libsdl1.2-dev 

*/



long double map(long double value, long double istart, long double istop, long double ostart, long double ostop){
  return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}



void setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
