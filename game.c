#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <time.h>
#include <math.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const float FPS = 60.0;

const int FONT_SIZE = 64;

const int PLAYER_SIZE = 48;
const int BACKGROUND_WIDTH = 1024;
const int GRASS_WIDTH = 700;
const int GRASS_HEIGHT = 70;
const int CACTUS_HEIGHT = 292;
const int CACTUS_WIDTH = 48;

const int SCORE_BUFFER_SIZE = 10;

const int CACTUS_BUFFER_SIZE = 100;

typedef struct {
    int x;
    int top;
    int gap;
} cactus;

//error reporting
void error(char *msg);

int main() {

    //handlers for allegro objects
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *player_bitmap = NULL;
    ALLEGRO_BITMAP *background_bitmap = NULL;
    ALLEGRO_BITMAP *grass_bitmap = NULL;
    ALLEGRO_BITMAP *cactus_bitmap = NULL;
    ALLEGRO_BITMAP *cactus_revert_bitmap = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_SAMPLE *jump_sound = NULL;
    ALLEGRO_SAMPLE *fail_sound = NULL;


    //initialize allegro stuff
    if (!al_init())
        error("Failed to initialize allegro");
        
    if (!al_init_image_addon())
        error("Failed to initialize image addon");
        
    if (!al_install_mouse())
        error("Failed to initialize mouse");
        
    al_init_font_addon();
        
    if (!al_init_ttf_addon())
        error("Failed to initialize ttf font");
        
    if(!al_install_audio())
        error("Failed to initialize audio");    
    
    if(!al_init_acodec_addon())
        error("Failed to initialize audio codecs");
    
    if(!al_reserve_samples(3))
        error("Failed to reserve samples");
    
    if ((display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT)) == 0)
        error("Failed to create display");
        
    if ((event_queue = al_create_event_queue()) == 0)
        error("Failed to create event queue");
    
    if ((timer = al_create_timer(1.0 / FPS)) == 0)
        error("Failed to create timer");
        
    if ((background_bitmap = al_load_bitmap("assets/background.png")) == 0)
        error("Failed to background bitmap");
    
    if ((grass_bitmap = al_load_bitmap("assets/grass.png")) == 0)
        error("Failed to grass bitmap");
        
    if ((player_bitmap = al_load_bitmap("assets/player.png")) == 0)
        error("Failed to player bitmap");
        
    if ((cactus_bitmap = al_load_bitmap("assets/cactus.png")) == 0)
        error("Failed to cactus bitmap");
        
    if ((cactus_revert_bitmap = al_load_bitmap("assets/cactus_revert.png")) == 0)
        error("Failed to cactus revert bitmap");
    
    if ((font = al_load_ttf_font("assets/font.ttf", FONT_SIZE, 0)) == 0)
        error("Failed to load ttf font");
    
    if ((jump_sound = al_load_sample("assets/jump.wav")) == 0)
        error("Failed to load jump sound");
        
    if ((fail_sound = al_load_sample("assets/fail.wav")) == 0)
        error("Failed to load fail sound");
    
    //allegro event
    ALLEGRO_EVENT ev;
    
    //add event listners and start timer
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    
    //game state, 0 end, 1 playing
    int play = 0;
    int reset = 1;
    
    //player score
    int score = 0;
    char score_buffer[SCORE_BUFFER_SIZE + 1];

    //player position
    float player_x = PLAYER_SIZE / 2;
    float player_y = SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2 - GRASS_HEIGHT / 2;
    
    //map offset
    float map_offset = 0;
    
    //speed
    float default_dx = 3;
    float default_dy = 2;
    
    //player chanage position
    float player_dx = default_dx;
    float player_dy = default_dy;
    
    //random cactus locations
    srand(time(0));
    int i;
    cactus cactus_location[CACTUS_BUFFER_SIZE];
    for (i = 0; i < CACTUS_BUFFER_SIZE; i++) {
        cactus_location[i].x = ((SCREEN_WIDTH / 2 - i) * (i + 1)) - CACTUS_WIDTH / 2;
        cactus_location[i].gap = 200 - (i + 1);
        cactus_location[i].top = (SCREEN_HEIGHT - GRASS_HEIGHT - cactus_location[i].gap) / 2 + rand() % 100 + 1 - 50;
    }
    
    //redraw watcher
    int redraw = true;
    
    //testing
    int passing = 0;
    
    while(1) {
        //wait for event
        al_wait_for_event(event_queue, &ev);
        
        //end game when user try to close window
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        
        //user click on scrren, get player up and play sound
        if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            //starting game
            if (play == 0 && reset == 0) {
                reset = 1;
                score = 0;
                player_x = PLAYER_SIZE / 2;
                player_y = SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2 - GRASS_HEIGHT / 2;
                player_dy = default_dy;
                player_dx = default_dx;
                map_offset = 0;
            } else if (reset) {
                reset = 0;
                play = 1;
            } else {
                //jumping
                player_dy = -4 * default_dy;
                al_play_sample(jump_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }
        
        //timer event, move player, check colisions
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            
            //if game is active
            if (play) {
                //move player or map if player reach half of screen
                if(player_x + PLAYER_SIZE / 2 < SCREEN_WIDTH / 2)
                    player_x += player_dx; 
                else
                    map_offset -= player_dx;
                player_y += player_dy;
                
                //slow down player
                if (player_dy < default_dy)
                    player_dy += default_dy * 0.5;
                
                //colision with grass
                if (player_y + PLAYER_SIZE >= SCREEN_HEIGHT - GRASS_HEIGHT)
                    play = 0;
                    
                //colision with sky
                if (player_y < 0)
                    play = 0;
                    
                //player_x position
                int player_pos_x = player_x - map_offset;
                    
                //colision with current cactus, first check rectangle colision, then check distance beetween middles
                if (player_pos_x + PLAYER_SIZE >= cactus_location[score].x && player_pos_x <= cactus_location[score].x + CACTUS_WIDTH) {
                    
                    //player y middle
                    int player_middle_y = player_y + PLAYER_SIZE / 2;
                    //player middle
                    int player_middle_x = player_pos_x + PLAYER_SIZE / 2;
                    
                    //top cactus middle
                    int top_cactus_middle_x = cactus_location[score].x + CACTUS_WIDTH / 2;
                    int top_cactus_middle_y = cactus_location[score].top - CACTUS_WIDTH / 2;
                    //bottom cactus middle
                    int bottom_cactus_middle_y = top_cactus_middle_y + cactus_location[score].gap + CACTUS_WIDTH;
                    
                    //if player middle is beetween top and bottom cactus we may pas or not, otherwise we fail for sure
                    if (player_middle_y >= top_cactus_middle_y && player_middle_y <= bottom_cactus_middle_y) {
                        //calc distance between player middle and cactus middle
                        
                        //distance beetween centers top
                        double distance_top = sqrt((player_middle_x - top_cactus_middle_x) * (player_middle_x - top_cactus_middle_x) + (player_middle_y - top_cactus_middle_y) * (player_middle_y - top_cactus_middle_y));
                        //distance beetween centers bottom
                        double distance_bottom = sqrt((player_middle_x - top_cactus_middle_x) * (player_middle_x - top_cactus_middle_x) + (player_middle_y - bottom_cactus_middle_y) * (player_middle_y - bottom_cactus_middle_y));
                        
                        if (distance_top <= PLAYER_SIZE || distance_bottom <= PLAYER_SIZE)
                            play = 0;
                    } else {
                        play = 0;
                    }
                }
                    
                //add score
                if (player_pos_x >= cactus_location[score].x + CACTUS_WIDTH)
                    score += 1;
                
                //fail
                if (play == 0)
                    al_play_sample(fail_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                
                //end game
                if (score == CACTUS_BUFFER_SIZE)
                    play = 0;
            }
            
            redraw = 1;
        }

        //redraw screen
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = 0;
            
            //draw background and next background tile, 0.75 paralax effect
            al_draw_bitmap(background_bitmap, (int)(map_offset * 0.75) % BACKGROUND_WIDTH, 0, 0);
            al_draw_bitmap(background_bitmap, BACKGROUND_WIDTH + ((int)(map_offset * 0.75) % BACKGROUND_WIDTH), 0, 0);
            
            //draw cactus
            for (i = score - 2; i < score + 3; i++) {
                if (i >= 0 && i < CACTUS_BUFFER_SIZE) {
                    al_draw_bitmap(cactus_revert_bitmap, cactus_location[i].x + map_offset, cactus_location[i].top - CACTUS_HEIGHT, 0);
                    al_draw_bitmap(cactus_bitmap, cactus_location[i].x + map_offset, cactus_location[i].top + cactus_location[i].gap, 0);
                }
            }
            
            //draw grass and next grass tile
            al_draw_bitmap(grass_bitmap, (int)map_offset % GRASS_WIDTH, SCREEN_HEIGHT - GRASS_HEIGHT, 0);
            al_draw_bitmap(grass_bitmap, GRASS_WIDTH + ((int)map_offset % GRASS_WIDTH), SCREEN_HEIGHT - GRASS_HEIGHT, 0);
            al_draw_bitmap(player_bitmap, player_x, player_y, 0);
            
            //draw score
            if (score == CACTUS_BUFFER_SIZE) {
                al_draw_text(font, al_map_rgb(205, 50, 50), SCREEN_WIDTH - (FONT_SIZE / 8), 0, ALLEGRO_ALIGN_RIGHT, "WIN");
            } else {
                snprintf(score_buffer, SCORE_BUFFER_SIZE, "%i", score);
                if (passing)
                    al_draw_text(font, al_map_rgb(255, 0, 0), 100, 100, ALLEGRO_ALIGN_CENTER, "NOW");
                    passing = 0;
                al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH - (FONT_SIZE / 8), 0, ALLEGRO_ALIGN_RIGHT, score_buffer);
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
    al_destroy_bitmap(cactus_bitmap);
    al_destroy_bitmap(cactus_revert_bitmap);
    al_destroy_font(font);
    al_destroy_sample(jump_sound);
    al_destroy_sample(fail_sound);
    
    return EXIT_SUCCESS;
}

void error(char *msg) {
    if (errno)
        fprintf(stderr, "Error: %s: %s\n", msg, strerror(errno));
    else
        fprintf(stderr, "Error: %s\n", msg);
        
    exit(EXIT_FAILURE);
}
