#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "bus6502.cpp"

using namespace std;


const int WIDTH = 1450;
const int HEIGHT = 480;
const int DEBUGGER_OFFSET = 512;

const bool ENABLE_DEBUGGER = true;
const int DEBUGGER_FONT_SIZE = 15;
// Disassembler view location.
const int DEBUGGER_DSB_Y_POS = 120;
const int DEBUGGER_DSB_X_POS = 470;
const int DEBUGGER_DSB_COUNT = 20;


class Viewer {
    private:
        int offset = 1000000;
        int offset_size = 100000;

    public:
        int width = WIDTH;
        int height = HEIGHT;

        // Debugger related.
        string mem_string = "";
        string dsb_string = "";
        string status_string = "";
        string register_string = "";
        string hint_string = "= C: STEP CPU = F: STEP FRAME = R: RESET = I: IRP (CPU) =\n= N: NMI (CPU) = SPACE: TOGGLE REALTIME [ ] = D: EXIT =";
        

        Bus *bus = nullptr;
        sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];

        Viewer(Bus * p_bus){
            // array of pixels
            //uint8_t pixels[this -> width * this -> height * 4];
            // memset(this -> frame, 0, sizeof this -> frame);
            for (int pi = 0; pi < WIDTH * HEIGHT * 4; pi += 4){
                pixels[pi] = 0;
                pixels[pi + 1] = 0;
                pixels[pi + 2] = 0;
                pixels[pi + 3] = 255;
            }
            bus = p_bus;
            update_debug_strings();
        };

        std::string hex(uint32_t n, uint8_t d)
        {
            std::string s(d, '0');
            for (int i = d - 1; i >= 0; i--, n >>= 4)
                s[i] = "0123456789ABCDEF"[n & 0xF];
            return s;
        };

        void update_debug_strings(){
            update_mem_string();
            update_dsb_string();
            update_status_register_string();
        }

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

        void draw_debugger_pane(
            sf::RenderWindow *window,
            sf::Font font
        ){
            // Memory view.
            sf::Text mem_view_sf_text(mem_string, font, DEBUGGER_FONT_SIZE);
            mem_view_sf_text.setPosition(0 + DEBUGGER_OFFSET, 0);
            window->draw(mem_view_sf_text);

            // Status and register view.
            sf::Text register_view_sf_text(register_string, font, DEBUGGER_FONT_SIZE);
            register_view_sf_text.setPosition(DEBUGGER_DSB_X_POS + DEBUGGER_OFFSET, 0);
            window->draw(register_view_sf_text);

            // Disassembler view.
            sf::Text dsb_text(dsb_string, font, DEBUGGER_FONT_SIZE);
            dsb_text.setPosition(DEBUGGER_DSB_X_POS + DEBUGGER_OFFSET, DEBUGGER_DSB_Y_POS);
            window->draw(dsb_text);

            // Hint view.
            sf::Text hint_text(hint_string, font, DEBUGGER_FONT_SIZE);
            hint_text.setPosition(DEBUGGER_DSB_X_POS + DEBUGGER_OFFSET, DEBUGGER_DSB_Y_POS + 320);
            window->draw(hint_text);


        }

        void update_mem_string(){
            // Change variables here.
            uint16_t start = 0x0000;
            uint16_t end = 0x01FF;
            uint8_t n_items = 16;


            mem_string.clear();
            for (uint16_t addr = start; addr < end; addr += n_items){
                mem_string += "$" + hex(addr, 4) + ":  ";
                for (uint8_t offset = 0; offset < n_items; offset++){
                    mem_string += hex(bus->cpu_ram[addr+offset], 2) + " ";
                }
                mem_string += "\n";
            }

        }

        void update_status_register_string(){
            status_string.clear();
            register_string.clear();

            // Format status string.
            int status_bools[8] = {
                bus->cpu.status & SY6502::N,
                bus->cpu.status & SY6502::V,
                bus->cpu.status & SY6502::U,
                bus->cpu.status & SY6502::B,
                bus->cpu.status & SY6502::D,
                bus->cpu.status & SY6502::I,
                bus->cpu.status & SY6502::Z,
                bus->cpu.status & SY6502::C,
            };

            status_string = "STATUS: N V - B D I Z C\n";
            string status_marks = "        ";
            for (int status_i = 0; status_i < 8; status_i++)
                if (status_bools[status_i] > 0)
                    status_marks += "X ";
                else
                    status_marks += "  ";
            

            register_string = status_string + status_marks + "\n" + register_string;
            
            // Format register string.
            uint16_t pc_content = bus->cpu.pc;
            uint8_t a_content = bus->cpu.a;
            uint8_t x_content = bus->cpu.x;
            uint8_t y_content = bus->cpu.y;
            uint16_t stkp_content = bus->cpu.stkp;
            uint8_t remaining_cycle = bus->cpu.cycles;

            register_string += "PC: $" + hex(pc_content, 4) + "\n";
            register_string += "A:  $" + hex(a_content, 2) + "    [" + to_string(a_content) + "]\n";
            register_string += "X:  $" + hex(x_content, 2) + "    [" + to_string(x_content) + "]\n";
            register_string += "Y:  $" + hex(y_content, 2) + "    [" + to_string(y_content) + "]\n";
            register_string += "Stack Pointer: $" + hex(stkp_content, 4) + "\n";
            register_string += "Remaining cycles: " + to_string(remaining_cycle) + "\n";
        }

        void update_dsb_string(){
            dsb_string.clear();
            dsb_string.append("\n");

            int lower_n = (int)DEBUGGER_DSB_COUNT / 2;  // Lines above current instruction.
            int upper_n = DEBUGGER_DSB_COUNT - 1 - lower_n;  // And below.

            map<uint16_t, std::string> mapped = bus->cpu.disassemble(0x0000, 0xFFFF);
            uint16_t current_addr = bus->cpu.pc;

            auto current_key = mapped.find(current_addr);
            auto current_key_u = current_key;
            // Lower spaces; starting from current key.
            if (current_key != mapped.end()){
                dsb_string += "-> " + (*current_key).second + "\n";
                for (int below = 0; below < lower_n; below++){
                    if (++current_key != mapped.end()){
                        dsb_string += "   " + (*current_key).second + "\n";
                    }
                }
            }
            // Upper spaces.
            if (current_key_u != mapped.begin()){
                for (int above = 0; above < upper_n; above++){
                    string line = "";
                    if (--current_key_u != mapped.begin()){
                        line = "   " + (*current_key_u).second;
                    }
                    dsb_string = "\n" + line + dsb_string;
                }
            }
            else
                for (int above = 0; above < upper_n; above++){
                    dsb_string = "\n" + dsb_string;
                }
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
            sf::Text text("Chicken text message\nBreakline", font, 20);
            text.setPosition(300, 300);
            // Load a music to play
            // sf::Music music;
            // if (!music.openFromFile("nice_music.ogg"))
            //     return EXIT_FAILURE;
        
            // Play the music
            // music.play();
            // Process events
            sf::Event event;
            sf::Texture frame_texture;
            frame_texture.create(WIDTH, HEIGHT);
            frame_texture.update(pixels);
            sf::Sprite sprite;
            sprite.setTexture(frame_texture);

            bool redraw_debugger = false;
        
            // Start the game loop
            while (window.isOpen())
            {
                redraw_debugger = false;
                while (window.pollEvent(event))
                {

                    // Close window: exit
                    switch (event.type) {
                        case sf::Event::Closed:
                            window.close();
                            break;

                        case sf::Event::KeyPressed:

                            redraw_debugger = true;

                            switch (event.key.code){
                                case sf::Keyboard::C:
                                    bus->cpu.clock();
                                    break;
                                case sf::Keyboard::F:
                                    cout << "F pressed" << endl;
                                    //bus->cpu.clock();
                                    break;
                                case sf::Keyboard::R:
                                    bus->cpu.reset();
                                    break;
                                case sf::Keyboard::I:
                                    bus->cpu.irq();
                                    break;
                                case sf::Keyboard::N:
                                    bus->cpu.nmi();
                                    break;
                                case sf::Keyboard::Space:
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

                    if (redraw_debugger)
                        update_debug_strings();

                    if (ENABLE_DEBUGGER)
                        draw_debugger_pane(&window, font);
            
                    // Update the window
                    window.display();
            }
        }
        return 0;
    }

};

