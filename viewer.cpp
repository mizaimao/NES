#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;


const int WIDTH = 640;
const int HEIGHT = 480;

namespace VIEWER{
    class Viewer {
        private:
            int offset = 1000000;
            int offset_size = 100000;

        public:
            int width = WIDTH;
            int height = HEIGHT;
            uint8_t frame[WIDTH * HEIGHT * 4];

            Viewer(){
                // array of pixels
                //uint8_t pixels[this -> width * this -> height * 4];
                memset(this -> frame, 0, sizeof this -> frame);
                
                for( int i = 200000; i < 900000; i++){
                    frame[i] = 100;
                }
                cout << frame[0] << endl;
                
            };

            /**
             * Put current frame into display.
            */
            int put_frame_a(SDL_Texture *p_texture){
                for(int i = offset - offset_size * 2; i < offset + offset_size * 2; i++){
                    this -> frame[i] = 0;
                }
                for(int i = offset - offset_size; i < offset; i++){
                    this -> frame[i] = 100;
                }
                // update texture with new data
                int texture_pitch = 0;
                void* p_texture_pixels = NULL;
                if (SDL_LockTexture(p_texture, NULL, &p_texture_pixels, &texture_pitch) != 0) {
                    SDL_Log("Unable to lock texture: %s", SDL_GetError());
                }
                else {
                    memcpy(p_texture_pixels, this -> frame, texture_pitch * this -> height);
                }
                SDL_UnlockTexture(p_texture);

                return 0;
            };

            int put_frame_b(SDL_Texture *p_texture){
                for(int i = offset - offset_size * 2; i < offset + offset_size * 2; i++){
                    this -> frame[i] = 0;
                }
                for(int i = offset; i < offset + offset_size; i++){
                    this -> frame[i] = 100;
                }
                // update texture with new data
                int texture_pitch = 0;
                void* p_texture_pixels = NULL;
                if (SDL_LockTexture(p_texture, NULL, &p_texture_pixels, &texture_pitch) != 0) {
                    SDL_Log("Unable to lock texture: %s", SDL_GetError());
                }
                else {
                    memcpy(p_texture_pixels, this -> frame, texture_pitch * this -> height);
                }
                SDL_UnlockTexture(p_texture);

                return 0;
            };


            /** 
             * Entry point to create the viewer and handle input events.
             **/
            int start_viewer(){

                SDL_Init(SDL_INIT_EVERYTHING);

                // Create an SDL window.
                SDL_Window *p_window = SDL_CreateWindow(
                    "Chicken Window", 
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED, 
                    this -> width, 
                    this -> height,
                    //SDL_WINDOW_RESIZABLE
                    SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
                );
                if (p_window == NULL){
                    SDL_Log("Unable to create window: %s", SDL_GetError());
                    return 1;      
                }

                // Create an SDL renderer.
                SDL_Renderer *p_renderer = SDL_CreateRenderer(
                    p_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
                );
                if (p_renderer == NULL) {
                    SDL_Log("Unable to create renderer: %s", SDL_GetError());
                    return 1;
                }
                SDL_RenderSetLogicalSize(p_renderer, this -> width, this -> height);

                // create texture
                SDL_Texture *p_texture = SDL_CreateTexture(
                    p_renderer,
                    SDL_PIXELFORMAT_RGBA32,
                    SDL_TEXTUREACCESS_STREAMING,
                    this -> width,
                    this -> height
                );
                if (p_texture == NULL) {
                    SDL_Log("Unable to create texture: %s", SDL_GetError());
                    return 1;
                }
            
                // update texture with new data
                int texture_pitch = 0;
                void* p_texture_pixels = NULL;
                if (SDL_LockTexture(p_texture, NULL, &p_texture_pixels, &texture_pitch) != 0) {
                    SDL_Log("Unable to lock texture: %s", SDL_GetError());
                }
                else {
                    memcpy(p_texture_pixels, this -> frame, texture_pitch * this -> height);
                }
                SDL_UnlockTexture(p_texture);

                // main loop
                bool should_quit = false;
                SDL_Event e;
                while (!should_quit) {

                    // for (int i = 0; i < this -> width * this -> height * 4; i ++){
                    //     pixels[i] = rand() % 255;
                    // }
                    // int texture_pitch = 0;
                    // void* p_texture_pixels = NULL;
                    // if (SDL_LockTexture(p_texture, NULL, &p_texture_pixels, &texture_pitch) != 0) {
                    //     SDL_Log("Unable to lock texture: %s", SDL_GetError());
                    // }
                    // else {
                    //     memcpy(p_texture_pixels, pixels, texture_pitch * this -> height);
                    // }
                    // SDL_UnlockTexture(p_texture);


                    while (SDL_PollEvent(&e) != 0) {
                        if (e.type == SDL_QUIT) {
                            should_quit = true;
                        } else if (e.type == SDL_KEYDOWN) {
                            if (e.key.keysym.sym == SDLK_UP) {
                            // Up Arrow
                                this -> put_frame_a(p_texture);

                            } else if (e.key.keysym.sym == SDLK_DOWN) {
                            // Down Arrow
                                this -> put_frame_b(p_texture);
                            } else if (e.key.keysym.sym == SDLK_LEFT) {
                            // Left Arrow
                            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                            // Right Arrow

                            //this opens a font style and sets a size
                                TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);

                                // this is the color in rgb format,
                                // maxing out all would give you the color white,
                                // and it will be your text's color
                                SDL_Color White = {255, 255, 255};

                                // as TTF_RenderText_Solid could only be used on
                                // SDL_Surface then you have to create the surface first
                                SDL_Surface* surfaceMessage =
                                    TTF_RenderText_Solid(Sans, "put your text here", White); 

                                // now you can convert it into a texture
                                SDL_Texture* Message = SDL_CreateTextureFromSurface(p_renderer, surfaceMessage);

                                SDL_Rect Message_rect; //create a rect
                                Message_rect.x = 0;  //controls the rect's x coordinate 
                                Message_rect.y = 0; // controls the rect's y coordinate
                                Message_rect.w = 100; // controls the width of the rect
                                Message_rect.h = 100; // controls the height of the rect

                                // (0,0) is on the top left of the window/screen,
                                // think a rect as the text's box,
                                // that way it would be very simple to understand

                                // Now since it's a texture, you have to put RenderCopy
                                // in your game loop area, the area where the whole code executes

                                // you put the renderer's name first, the Message,
                                // the crop size (you can ignore this if you don't want
                                // to dabble with cropping), and the rect which is the size
                                // and coordinate of your texture
                                SDL_RenderCopy(p_renderer, Message, NULL, &Message_rect);

                                // Don't forget to free your surface and texture
                                SDL_FreeSurface(surfaceMessage);
                                SDL_DestroyTexture(Message);
                            }
                        }
                    }

                    // render on screen
                    SDL_RenderClear(p_renderer);
                    SDL_RenderCopy(p_renderer, p_texture, NULL, NULL);
                    SDL_RenderPresent(p_renderer);
                }

                SDL_DestroyTexture(p_texture);
                SDL_DestroyRenderer(p_renderer);
                SDL_DestroyWindow(p_window);
                SDL_Quit();

                return 0;
            };
    };

}
