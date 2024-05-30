#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "game.hpp"
#include "player.hpp"
#include "cactus.hpp"
#include "agent.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const float FPS = 60.0;

const int FONT_SIZE = 64;

const int BACKGROUND_WIDTH = 1024;
const int GRASS_WIDTH = 700;
const int GRASS_HEIGHT = 70;

const int SCORE_BUFFER_SIZE = 10;

const int CACTUS_BUFFER_SIZE = 100;

int main(int argc, char* argv[]) {
    bool use_agent = false;
    if (argc > 1) {
        if (strncmp(argv[1], "--agent", 7) == 0) {
            use_agent = true;
        }
    }

    int frame_to_step = FPS / 3;

    //handlers for allegro objects
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *player_bitmap = NULL;
    ALLEGRO_BITMAP *background_bitmap = NULL;
    ALLEGRO_BITMAP *grass_bitmap = NULL;
    ALLEGRO_BITMAP *cactus_bitmap[2] = {NULL, NULL};
    ALLEGRO_FONT *font = NULL;

    //initialize allegro stuff
    if (!al_init())
        perror("Failed to initialize allegro");
        
    if (!al_init_image_addon())
        perror("Failed to initialize image addon");
        
    if (!al_install_mouse())
        perror("Failed to initialize mouse");
        
    al_init_font_addon();
        
    if (!al_init_ttf_addon())
        perror("Failed to initialize ttf font");
    
    if ((display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT)) == 0)
        perror("Failed to create display");
        
    if ((event_queue = al_create_event_queue()) == 0)
        perror("Failed to create event queue");
    
    if ((timer = al_create_timer(1.0 / FPS)) == 0)
        perror("Failed to create timer");
        
    if ((background_bitmap = al_load_bitmap("assets/background.png")) == 0)
        perror("Failed to background bitmap");
    
    if ((grass_bitmap = al_load_bitmap("assets/grass.png")) == 0)
        perror("Failed to grass bitmap");
        
    if ((player_bitmap = al_load_bitmap("assets/player.png")) == 0)
        perror("Failed to player bitmap");
        
    if ((cactus_bitmap[0] = al_load_bitmap("assets/cactus.png")) == 0)
        perror("Failed to cactus bitmap");
        
    if ((cactus_bitmap[1] = al_load_bitmap("assets/cactus_revert.png")) == 0)
        perror("Failed to cactus revert bitmap");
    
    if ((font = al_load_ttf_font("assets/font.ttf", FONT_SIZE, 0)) == 0)
        perror("Failed to load ttf font");
    
    //allegro event
    ALLEGRO_EVENT ev;
    
    //add event listeners and start timer
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    
    //game state, 0 end, 1 playing
    bool play = false;
    bool reset = true;

    //player
    Player player = Player(Player::SIZE / 2, SCREEN_HEIGHT / 2 - Player::SIZE / 2 - GRASS_HEIGHT / 2, player_bitmap);
    
    //map offset
    float map_offset = 0;
    
    //random cactus locations
    srand(time(0));
    Cactus cactuses[CACTUS_BUFFER_SIZE];
    for (int i = 0; i < CACTUS_BUFFER_SIZE; i++) {
        cactuses[i] = Cactus(((SCREEN_WIDTH / 2 - i) * (i + 1)) - Cactus::WIDTH / 2, 200 - (i + 1), cactus_bitmap);
    }

    // AI agent
    Agent agent = Agent();
    
    //redraw watcher
    bool redraw = true;
    
    int frame = 0;
    int step = 0;

    while(true) {
        //wait for event
        al_wait_for_event(event_queue, &ev);
        
        //end game when user try to close window
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        
        //user click on scrren, get player up and play sound
        if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            //starting game
            if (play == false && reset == false) {
                reset = true;
                player.setLeft(Player::SIZE / 2);
                player.setTop(SCREEN_HEIGHT / 2 - Player::SIZE / 2 - GRASS_HEIGHT / 2);
                player.resetSpeed();
                map_offset = 0;
                player.resetScore();
                for (int i = 0; i < CACTUS_BUFFER_SIZE; i++) {
                    cactuses[i].randomTop();
                }
            } else if (reset) {
                reset = false;
                play = true;
            } else {
                //jumping
                if (!use_agent) {
                    player.jump();
                }
            }
        }
        
        //timer event, move player, check colisions
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            bool is_step = (frame % frame_to_step) == 0;
            //if game is active
            if (play) {
                //environment code
                int observation[] = {cactuses[player.getScore()].getX() - player.getRight(), cactuses[player.getScore()].getBottom() - player.getTop() - Player::SIZE};
                bool action = 0;

                if (is_step) {
                    //agent action
                    if (use_agent) {
                        // neutral net agent
                        action = agent.act(observation);

                        if (action) {
                            player.jump();
                        }
                    }
                }

                //move player or map if player reach half of screen
                if(player.getLeft() + Player::SIZE / 2 >= SCREEN_WIDTH / 2)
                    map_offset -= player.getDx();
                    
                player.timer();
                
                //colision with grass
                if (player.getBottom() >= SCREEN_HEIGHT - GRASS_HEIGHT)
                    play = false;
                    
                //colision with sky
                if (player.getTop() < 0)
                    play = false;
                    
                //colision with current cactus, first check rectangle colision, then check distance between middles
                if (player.getRight() >= cactuses[player.getScore()].getX() && player.getLeft() <= cactuses[player.getScore()].getX() + Cactus::WIDTH) {
                    
                    //player y middle
                    int player_middle_y = player.getTop() + Player::SIZE / 2;
                    //player middle
                    int player_middle_x = player.getLeft() + Player::SIZE / 2;
                    
                    //top cactus middle
                    int top_cactus_middle_x = cactuses[player.getScore()].getX() + Cactus::WIDTH / 2;
                    int top_cactus_middle_y = cactuses[player.getScore()].getTop() - Cactus::WIDTH / 2;
                    //bottom cactus middle
                    int bottom_cactus_middle_y = top_cactus_middle_y + cactuses[player.getScore()].getGap() + Cactus::WIDTH;
                    
                    //if player middle is between top and bottom cactus we may pas or not, otherwise we fail for sure
                    if (player_middle_y >= top_cactus_middle_y && player_middle_y <= bottom_cactus_middle_y) {
                        //calc distance between player middle and cactus middle
                        
                        //distance between centers top
                        double distance_top = sqrt((player_middle_x - top_cactus_middle_x) * (player_middle_x - top_cactus_middle_x) + (player_middle_y - top_cactus_middle_y) * (player_middle_y - top_cactus_middle_y));
                        //distance between centers bottom
                        double distance_bottom = sqrt((player_middle_x - top_cactus_middle_x) * (player_middle_x - top_cactus_middle_x) + (player_middle_y - bottom_cactus_middle_y) * (player_middle_y - bottom_cactus_middle_y));
                        
                        if (distance_top <= Player::SIZE || distance_bottom <= Player::SIZE)
                            play = false;
                    } else {
                        play = false;
                    }
                }
                    
                //add score
                if (player.getLeft() >= cactuses[player.getScore()].getX() + Cactus::WIDTH)
                    player.incScore();

                //end game
                if (player.getScore() == CACTUS_BUFFER_SIZE)
                    play = false;

                //environment code
                int reward = 1;
                bool done = 0;
                if (!play) {
                    done = 1;
                    if (player.getScore() < CACTUS_BUFFER_SIZE) {
                        reward = -100;
                    }
                }

                if (is_step || done) {
                    //agent learn
                    int next_observation[] = {cactuses[player.getScore()].getX() - player.getRight(), cactuses[player.getScore()].getBottom() - player.getTop() - Player::SIZE};
                    if (use_agent) {
                        agent.learn(observation, action, reward, next_observation, done);
                    }
                    step++;
                }

                if (done) {
                    step = 0;
                }
            }
            
            redraw = true;
        }

        //redraw screen
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            
            //draw background and next background tile, 0.75 paralax effect
            al_draw_bitmap(background_bitmap, (int)(map_offset * 0.75) % BACKGROUND_WIDTH, 0, 0);
            al_draw_bitmap(background_bitmap, BACKGROUND_WIDTH + ((int)(map_offset * 0.75) % BACKGROUND_WIDTH), 0, 0);
            
            //draw cactus
            for (int i = player.getScore() - 2; i < player.getScore() + 3; i++) {
                if (i >= 0 && i < CACTUS_BUFFER_SIZE) {
                    cactuses[i].draw(map_offset);
                }
            }
            
            //draw grass and next grass tile
            al_draw_bitmap(grass_bitmap, (int)map_offset % GRASS_WIDTH, SCREEN_HEIGHT - GRASS_HEIGHT, 0);
            al_draw_bitmap(grass_bitmap, GRASS_WIDTH + ((int)map_offset % GRASS_WIDTH), SCREEN_HEIGHT - GRASS_HEIGHT, 0);
            player.draw(map_offset);
            
            //draw score
            if (player.getScore() == CACTUS_BUFFER_SIZE) {
                al_draw_text(font, al_map_rgb(205, 50, 50), SCREEN_WIDTH - (FONT_SIZE / 8), 0, ALLEGRO_ALIGN_RIGHT, "WIN");
            } else {
                al_draw_textf(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH - (FONT_SIZE / 8), 0, ALLEGRO_ALIGN_RIGHT, "%i", player.getScore());
            }
            
            //change buffer
            al_flip_display();
        }
    }
    
    //allegro clean ups
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_bitmap(background_bitmap);
    al_destroy_bitmap(grass_bitmap);
    al_destroy_bitmap(player_bitmap);
    al_destroy_bitmap(cactus_bitmap[0]);
    al_destroy_bitmap(cactus_bitmap[1]);
    al_destroy_font(font);
    
    return EXIT_SUCCESS;
}
