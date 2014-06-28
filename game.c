#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const float FPS = 60.0;
const int BOUNCER_SIZE = 32;
const int FONT_SIZE = 64;

void error(char *msg);

int main(int argc, char **argv) {

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *bouncer[2] = {NULL, NULL};
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_SAMPLE *boing = NULL;
    
    bool redraw = true;
    int red = 0;
    char buffer[11] = {0};
    int count = 0;
    int size = 0;
    
    float bouncer_x = SCREEN_WIDTH / 2.0 - BOUNCER_SIZE / 2.0;
    float bouncer_y = SCREEN_HEIGHT / 2.0 - BOUNCER_SIZE / 2.0;
    float bouncer_dx = -2.0;
    float bouncer_dy = 2.0;

    if (!al_init())
        error("Failed to initialize allegro");
        
    if (!al_init_image_addon())
        error("Failed to initialize image addon");
        
    if (!al_install_mouse())
        error("Failed to initialize mouse");
        
    if (!al_install_keyboard())
        error("Failed to initialize keyboard");
        
    al_init_font_addon();
        
    if (!al_init_ttf_addon())
        error("Failed to initialize ttf font");
        
    if(!al_install_audio())
        error("Failed to initialize audio");    
    
    if(!al_init_acodec_addon())
        error("Failed to initialize audio codecs");
    
    if(!al_reserve_samples(2))
        error("Failed to reserve samples");
    
    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display)
        error("Failed to create display");
        
    event_queue = al_create_event_queue();
    if (!event_queue) {
        al_destroy_display(display);
        error("Failed to create event queue");
    }
    
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        error("Failed to create timer");
    }
    
    bouncer[0] = al_load_bitmap("ball.png");
    if (!bouncer) {
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        error("Failed to bouncer bitmap");
    }
    
    bouncer[1] = al_load_bitmap("ball_red.png");
    if (!bouncer) {
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        al_destroy_bitmap(bouncer[0]);
        error("Failed to red bouncer bitmap");
    }
    
    font = al_load_ttf_font("font.ttf", FONT_SIZE, 0);
    if (!font){
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        al_destroy_bitmap(bouncer[0]);
        al_destroy_bitmap(bouncer[1]);
        error("Could not load ttf font");
    }
    
    boing = al_load_sample("assets/jump.wav");
    if (!boing) {
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        al_destroy_bitmap(bouncer[0]);
        al_destroy_bitmap(bouncer[1]);
        al_destroy_font(font);
        error("Audio clip sample not loaded");
    }
    
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
        
    al_clear_to_color(al_map_rgb(50, 205, 50));
    
    al_flip_display();
    
    al_start_timer(timer);
    
    while(1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (bouncer_x < 0 || bouncer_x > SCREEN_WIDTH - BOUNCER_SIZE) {
                bouncer_dx = -bouncer_dx;
                al_play_sample(boing, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                count++;
            }
            
            if (bouncer_y < 0 || bouncer_y > SCREEN_HEIGHT - BOUNCER_SIZE) {
                bouncer_dy = -bouncer_dy;
                al_play_sample(boing, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                count++;
            }
            
            bouncer_x += bouncer_dx;
            bouncer_y += bouncer_dy;
            
            redraw = true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (ev.mouse.x >= bouncer_x
                && ev.mouse.x < bouncer_x + BOUNCER_SIZE
                && ev.mouse.y >= bouncer_y
                && ev.mouse.y < bouncer_y + BOUNCER_SIZE)
                red = (red + 1) % 2;
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_R) {
            red = 1;
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_Y) {
            red = 0;
        }
        
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(50, 205, 50));
            size = snprintf(buffer, 10, "%i", count);
            buffer[size] = '\0';
            al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - (FONT_SIZE / 2), ALLEGRO_ALIGN_CENTRE, buffer);
            al_draw_bitmap(bouncer[red], bouncer_x, bouncer_y, 0);
            al_flip_display();
        }
    }
    
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_bitmap(bouncer[0]);
    al_destroy_bitmap(bouncer[1]);
    al_destroy_font(font);
    al_destroy_sample(boing);
    
    return EXIT_SUCCESS;
}

void error(char *msg) {
    if (errno)
        fprintf(stderr, "Error: %s: %s\n", msg, strerror(errno));
    else
        fprintf(stderr, "Error: %s\n", msg);
    exit(EXIT_FAILURE);
}
