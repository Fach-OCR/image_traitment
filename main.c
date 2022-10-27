#include "utilis_image.h"
#include "otsu.h"
#include "sobel.h"
#include "gaussian_filter.h"

#include <err.h>

void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_RenderPresent(renderer);
}


void event_loop(SDL_Renderer* renderer, Image* image1)
{
    SDL_Event e;

    int highthresh = 200;
    int lowtresh = 100;


    SDL_Surface* surface;
    SDL_Texture* texture;
    Image image;

    while( 1 )
    {
        SDL_WaitEvent(&e);
        if (e.type == SDL_QUIT)
            break;
        else if( e.type == SDL_KEYDOWN )
        {
            switch( e.key.keysym.sym )
            {
                case SDLK_UP:
                    if (highthresh < 251)
                        highthresh += 5;

                    printf("highthresh = %i\n", highthresh);
                    printf("lowtrsh = %i\n", lowtresh);
                    printf("\n\n\n\n");

                    image = copy_image(image1);
                    double_threshold(&image, highthresh, lowtresh);
                    hysteris(&image);

                    surface = create_surface(&image);
                    texture = SDL_CreateTextureFromSurface(renderer, surface);
                    draw(renderer, texture);

                    break;

                case SDLK_DOWN:
                    highthresh -= 5;

                    printf("highthresh = %i\n", highthresh);
                    printf("lowtrsh = %i\n", lowtresh);
                    printf("\n\n\n\n");

                    image = copy_image(image1);
                    double_threshold(&image, highthresh, lowtresh);
                    hysteris(&image);

                    surface = create_surface(&image);
                    texture = SDL_CreateTextureFromSurface(renderer, surface);
                    draw(renderer, texture);

                    break;

                case SDLK_LEFT:
                    lowtresh -= 5;

                    printf("highthresh = %i\n", highthresh);
                    printf("lowtrsh = %i\n", lowtresh);
                    printf("\n\n\n\n");

                    image = copy_image(image1);
                    double_threshold(&image, highthresh, lowtresh);
                    hysteris(&image);

                    surface = create_surface(&image);
                    texture = SDL_CreateTextureFromSurface(renderer, surface);
                    draw(renderer, texture);

                    break;

                case SDLK_RIGHT:
                    if (lowtresh < 251)
                        lowtresh += 5;

                    printf("highthresh = %i\n", highthresh);
                    printf("lowtrsh = %i\n", lowtresh);
                    printf("\n\n\n\n");

                    image = copy_image(image1);
                    double_threshold(&image, highthresh, lowtresh);
                    hysteris(&image);

                    surface = create_surface(&image);
                    texture = SDL_CreateTextureFromSurface(renderer, surface);
                    draw(renderer, texture);

                    break;


                default:
                    break;
            }
        }
    }
}


int main(int argc, char** argv)
{
    if (argc > 3)
        errx(EXIT_FAILURE, "Usage: image-file");

    if (argc == 3) // Manual threshold mode
    {
        // SDL Stuff
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_Window* window = SDL_CreateWindow("Dynamic Fractal Canopy", 0, 0, 640, 400, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (window == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        SDL_Texture* texture = IMG_LoadTexture(renderer, argv[1]);
        if (texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());


        SDL_Surface* surface = IMG_Load(argv[1]);
        SDL_SetWindowSize(window, surface->w, surface->h);

        Image image = create_image(surface, surface->w, surface->h);

        image.path = (char *)calloc(strlen(argv[1]) + 5, sizeof(char));
        image.path[0] = 'r';
        image.path[1] = 'e';
        image.path[2] = 's';
        image.path[3] = '_';
        strcat(image.path, argv[1]);


        printf("%s", image.path);
        SDL_FreeSurface(surface);

        surface_to_grayscale(&image);
        gaussian_blur(&image, 3);
        edges(&image);

        event_loop(renderer, &image);


        // Destroy the components used to free memory
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        freeImage(&image);
    }
    else
    {
        // Import image
        SDL_Surface* surface = IMG_Load(argv[1]);
        Image image = create_image(surface, surface->w, surface->h);

        // Create the name to save image
        image.path = (char *)calloc(strlen(argv[1]) + 5, sizeof(char));
        image.path[0] = 'r';
        image.path[1] = 'e';
        image.path[2] = 's';
        image.path[3] = '_';
        strcat(image.path, argv[1]);

        // Perform canny on the image
        surface_to_grayscale(&image);
        gaussian_blur(&image, 3);
        apply_threshold(&image, otsu(&image));
        //edges(&image);
        //double_threshold(&image, 240, 100);
        //hysteris(&image);

        // Save the image
        SDL_Surface* final_surface = create_surface(&image);
        SDL_SaveBMP(final_surface, image.path);

        // Free image and surface
        SDL_FreeSurface(final_surface);
        freeImage(&image);

    }
    SDL_Quit();

    return EXIT_SUCCESS;
   }
