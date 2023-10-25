#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace std;


const int WIDTH = 1280;
const int HEIGHT = 480;
const int DEBUGGER_OFFSET = 640;

const bool ENABLE_DEBUGGER = true;


class Viewer {
    private:
        int offset = 1000000;
        int offset_size = 100000;

    public:
        int width = WIDTH;
        int height = HEIGHT;
        sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];

        Viewer(){
            // array of pixels
            //uint8_t pixels[this -> width * this -> height * 4];
            // memset(this -> frame, 0, sizeof this -> frame);
            for (int pi = 0; pi < WIDTH * HEIGHT * 4; pi += 4){
                pixels[pi] = 0;
                pixels[pi + 1] = 0;
                pixels[pi + 2] = 0;
                pixels[pi + 3] = 255;
            }
        };

        // Fill the pixel array with black RGB and 255 alpha.
        void fill_black(){
            for (int pi = 0; pi < WIDTH * HEIGHT * 4; pi += 4){
                pixels[pi] = 0;
                pixels[pi + 1] = 0;
                pixels[pi + 2] = 0;
                pixels[pi + 3] = 255;
            }
        }

        void draw_pixel(int x, int y, int fill){
            int location = y * WIDTH * 4 + x * 4;
            pixels[location] = fill;
            pixels[location + 1 ] = fill;
            pixels[location + 2 ] = fill;
        }

        void draw_pixel(int x, int y, uint8_t color[3]){
            int location = y * WIDTH * 4 + x * 4;
            pixels[location] = color[0];
            pixels[location + 1 ] = color[1];
            pixels[location + 2 ] = color[2];
        }

        void draw_ract(int x_start, int x_end, int y_start, int y_end, int fill){
            for ( int x = x_start; x < x_end; x ++){
                for (int y = y_start; y < y_end; y ++){
                    draw_pixel(x, y, fill);
                }
            }
        }

        void draw_ract(int x_start, int x_end, int y_start, int y_end, uint8_t color[3]){
            for ( int x = x_start; x < x_end; x ++){
                for (int y = y_start; y < y_end; y ++){
                    draw_pixel(x, y, color);
                }
            }
        }

        void upper_block(){
            fill_black();
            draw_ract(300, 400, 100, 200, (uint8_t[3]){100, 200, 100});
        }

        void lower_block(){
            fill_black();
            draw_ract(300, 400, 300, 400, (uint8_t[3]){200, 100, 100});
        }

        void left_block(){
            fill_black();
            draw_ract(100, 200, 300, 400, (uint8_t[3]){100, 100, 200});
        }

        void right_block(){
            fill_black();
            draw_ract(500, 600, 300, 400, (uint8_t[3]){100, 200, 200});
        }

        int launch_window(){
            // Create the main window
            sf::RenderWindow window(
                sf::VideoMode(WIDTH, HEIGHT),
                "Chicken Window",
                // sf::Style::Default combines these three.
                sf::Style::Resize | sf::Style::Titlebar | sf::Style::Close
            );

            // Control FPS here.
            window.setFramerateLimit(60);
        
            // Load a sprite to display
            sf::Texture texture;
            // if (!texture.loadFromFile("cute_image.jpg"))
            //     return EXIT_FAILURE;
            // sf::Sprite sprite(texture);
        
            // Create a graphical text to display
            sf::Font font;
            if (!font.loadFromFile("PerfectDOSVGA437.ttf"))
                return EXIT_FAILURE;
            sf::Text text("Chicken text message", font, 20);
            text.setPosition(300, 300);
            // Load a music to play
            // sf::Music music;
            // if (!music.openFromFile("nice_music.ogg"))
            //     return EXIT_FAILURE;
        
            // Play the music
            // music.play();

            cout <<"CHICKEN"<<endl;

            // Process events
            sf::Event event;
            sf::Texture frame_texture;
            frame_texture.create(WIDTH, HEIGHT);
            frame_texture.update(pixels);
            sf::Sprite sprite;
            sprite.setTexture(frame_texture);
        
            // Start the game loop
            while (window.isOpen())
            {
                while (window.pollEvent(event))
                {

                    // Close window: exit
                    switch (event.type) {
                        case sf::Event::Closed:
                            window.close();
                            break;

                        case sf::Event::KeyPressed:
                            switch (event.key.code){
                                case sf::Keyboard::N:
                                    cout << "N is pressed" << endl;
                                    break;
                                case sf::Keyboard::D:
                                    window.close();
                                    break;
                                case sf::Keyboard::Up:
                                    upper_block();
                                    frame_texture.update(pixels);
                                    sprite.setTexture(frame_texture);
                                    break;
                                case sf::Keyboard::Down:
                                    lower_block();
                                    frame_texture.update(pixels);
                                    sprite.setTexture(frame_texture);
                                    break;

                                case sf::Keyboard::Left:
                                    left_block();
                                    frame_texture.update(pixels);
                                    sprite.setTexture(frame_texture);
                                    break;

                                case sf::Keyboard::Right:
                                    right_block();
                                    frame_texture.update(pixels);
                                    sprite.setTexture(frame_texture);
                                    break;

                                default:
                                    break;

                            }
                            break;

                        default:
                            break;
                    }
        
                    // Clear screen
                    window.clear();
            
                    // Draw the sprite
                    window.draw(sprite);
            
                    // Draw the string
                    window.draw(text);
            
                    // Update the window
                    window.display();
            }
        }
        return 0;
    }

};

