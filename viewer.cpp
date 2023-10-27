#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "bus6502.cpp"

using namespace std;

const float TARGET_FPS = 60.0f;

const int WIDTH = 1450;
const int HEIGHT = 480;
const int DEBUGGER_OFFSET = 512 + 10;

const bool ENABLE_DEBUGGER = true;
const int DEBUGGER_FONT_SIZE = 15;
// Disassembler view location.
const int DEBUGGER_DSB_Y_POS = 120;
const int DEBUGGER_DSB_X_POS = 470;
const int DEBUGGER_DSB_COUNT = 20;
const sf::Color DEBUGGER_TEXT_COLOR = sf::Color(200, 200, 200, 255);


class Viewer {
    private:
        int frame_counter = 0;

    public:
        int width = WIDTH;
        int height = HEIGHT;

        // Debugger related.
        string mem_string = "";
        string dsb_string = "";
        string status_string = "";
        string register_string = "";
        string hint_string = "";
        

        Bus *bus = nullptr;
        sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];
        // Pixel array but in shader format.
        sf::VertexArray ppu_image_va;

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

            // Initialize vertex array.
            ppu_image_va.setPrimitiveType(sf::Triangles);
            ppu_image_va.resize(256 * 240 * 6);
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
            update_hint_string();
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

        // Copy sprite_screen from PPU to pixel array.
        // Copy from RGB to RBGA.
        void draw_from_ppu(int zoom = 1){
            cout << "CALLED " << frame_counter  <<endl;

            for (int y = 0; y < 240; y ++){
                for (int x = 0; x < 256; x ++){
                    int src_index = y * 256 * 3 + x * 3;
                    int tgt_index = y * 256 * 4 + x * 4;

                    int pixel_offset = y * (WIDTH - 256 * zoom) * 4;
                    
                    pixels[pixel_offset + tgt_index] = bus -> ppu.sprite_screen[src_index];
                    pixels[pixel_offset + tgt_index + 1] = bus -> ppu.sprite_screen[src_index + 1];
                    pixels[pixel_offset + tgt_index + 2] = bus -> ppu.sprite_screen[src_index + 2];
                }
            }
        }
        void add_pixel_shader(int x, int y, sf::Color pixel_color, float scale){
            int tgt_index = (y * 256 + x) * 6;  // Two triangles, six vertices.
            sf::Vertex* triangles = &ppu_image_va[tgt_index];
            triangles[0].color = pixel_color;
            triangles[1].color = pixel_color;
            triangles[2].color = pixel_color;
            triangles[3].color = pixel_color;
            triangles[4].color = pixel_color;
            triangles[5].color = pixel_color;

            triangles[0].position = sf::Vector2f( x * scale, y * scale) ;
            triangles[1].position = sf::Vector2f( x * scale, (y + 1) * scale );
            triangles[2].position = sf::Vector2f( (x + 1) * scale, y * scale );
            triangles[3].position = sf::Vector2f( (x + 1) * scale, y * scale );
            triangles[4].position = sf::Vector2f( (x + 1) * scale, (y + 1) * scale );
            triangles[5].position = sf::Vector2f( x * scale, (y + 1) * scale );
        }

        void draw_from_ppu_shader(float scale = 2.0f){

            if (ppu_image_va.getVertexCount() == 0){
                cout << "Vertex count is zero." << endl;
            }

            for (int y = 0; y < 240; y ++){
                for (int x = 0; x < 256; x ++){
                    int src_index = y * 256 * 3 + x * 3;
                    
                    sf::Color pixel_color(
                        bus -> ppu.sprite_screen[src_index],
                        bus -> ppu.sprite_screen[src_index + 1],
                        bus -> ppu.sprite_screen[src_index + 2]
                    );
                    add_pixel_shader(x, y, pixel_color, scale);
                    //cout << x << " " << y << endl;
                    
                }
            }
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
            mem_view_sf_text.setFillColor(DEBUGGER_TEXT_COLOR);
            window->draw(mem_view_sf_text);

            // Status and register view.
            sf::Text register_view_sf_text(register_string, font, DEBUGGER_FONT_SIZE);
            register_view_sf_text.setPosition(DEBUGGER_DSB_X_POS + DEBUGGER_OFFSET, 0);
            register_view_sf_text.setFillColor(DEBUGGER_TEXT_COLOR);
            window->draw(register_view_sf_text);

            // Disassembler view.
            sf::Text dsb_text(dsb_string, font, DEBUGGER_FONT_SIZE);
            dsb_text.setPosition(DEBUGGER_DSB_X_POS + DEBUGGER_OFFSET, DEBUGGER_DSB_Y_POS);
            dsb_text.setFillColor(DEBUGGER_TEXT_COLOR);
            window->draw(dsb_text);

            // Hint view.
            sf::Text hint_text(hint_string, font, DEBUGGER_FONT_SIZE);
            hint_text.setPosition(DEBUGGER_DSB_X_POS + DEBUGGER_OFFSET, DEBUGGER_DSB_Y_POS + 320);
            hint_text.setFillColor(DEBUGGER_TEXT_COLOR);
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
            register_string += "System clock count: " + to_string(bus->system_clock_counter) + "\n";
            register_string += "Window frame draw: " + to_string(frame_counter) + "\n";
            register_string += "PPU frame draw: " + to_string(bus->ppu.ppu_frame_draw) + "\n";
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

        void update_hint_string(){
            hint_string.clear();
            hint_string += "= C: STEP CPU = F: STEP FRAME = R: RESET = I: IRP (CPU) =\n= N: NMI (CPU) = SPACE: TOGGLE REALTIME [";
            if (bus->ppu.is_emulation_run)
                hint_string.append("X");
            else
                hint_string.append(" ");
            hint_string +=  "] = D: EXIT =";
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
            text.setPosition(0, 0);
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

            // Start the game loop
            while (window.isOpen())
            {
                if (bus->ppu.is_emulation_run){
                    do { bus->clock(); } while ( not bus->ppu.frame_complete);
                    bus->ppu.frame_complete = false;
                }
                while (window.pollEvent(event))
                {
                    switch (event.type) {
                        // Close window: exit
                        case sf::Event::Closed:
                            window.close();
                            break;

                        case sf::Event::KeyPressed:
                            switch (event.key.code){
                                case sf::Keyboard::C:
                                    // CPU ticks every three times PPU and system ticks.
                                    // Additional loop to drain out CPU cycles.
                                    do { bus->clock(); } while ( not bus->cpu.complete());
                                    do { bus->clock(); } while ( not bus->cpu.complete());
                                    //std::cout << "Loading at 0: " << std::to_string(bus->ppu.sprite_screen[0]) << std::endl;
                                    break;
                                case sf::Keyboard::F:
                                    do { bus->clock(); } while ( not bus->ppu.frame_complete);
                                    do { bus->clock(); } while ( not bus->cpu.complete());
                                    bus->ppu.frame_complete = false; 
                                    //bus->cpu.clock();
                                    break;
                                case sf::Keyboard::R:
                                    bus->reset();
                                    break;
                                case sf::Keyboard::I:
                                    bus->cpu.irq();
                                    break;
                                case sf::Keyboard::N:
                                    bus->cpu.nmi();
                                    break;
                                case sf::Keyboard::Space:
                                    bus->ppu.is_emulation_run = not bus->ppu.is_emulation_run;
                                    break;
                                case sf::Keyboard::D:
                                    window.close();
                                    break;
                                case sf::Keyboard::Up:
                                    upper_block();
                                    break;
                                case sf::Keyboard::Down:
                                    lower_block();
                                    break;

                                case sf::Keyboard::Left:
                                    left_block();
                                    break;

                                case sf::Keyboard::Right:
                                    right_block();
                                    break;

                                default:
                                    break;

                            }
                            break;

                        default:
                            break;
                    }
                }
                // Clear screen
                window.clear();
        
                // Draw the sprite
                // draw_from_ppu();
                frame_texture.update(pixels);
                sprite.setTexture(frame_texture);
                window.draw(sprite);

                // Draw ppu as shader.
                draw_from_ppu_shader();
                window.draw(ppu_image_va);
        
                // Draw the string
                window.draw(text);
                update_debug_strings();

                sf::VertexArray triangle(sf::Triangles, 3);

                if (ENABLE_DEBUGGER)
                    draw_debugger_pane(&window, font);
        
                // Update the window
                window.display();
                frame_counter++;
            }
        return 0;
        }

};

