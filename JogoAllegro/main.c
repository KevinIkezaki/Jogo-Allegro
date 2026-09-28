#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#define LARGURA 1920
#define ALTURA  1080
#define FPS     60.0

int main(void)
{
    /* 1. Inicializacao da biblioteca e dos modulos */
    if (!al_init()) {
        fprintf(stderr, "Falha ao iniciar o Allegro.\n");
        return 1;
    }
    if (!al_install_keyboard() || !al_init_primitives_addon() || !al_init_font_addon()) {
        fprintf(stderr, "Falha ao iniciar teclado ou add-ons.\n");
        return 1;
    }

    /* 2. Criacao dos recursos */
    ALLEGRO_DISPLAY     *janela = al_create_display(LARGURA, ALTURA);
    ALLEGRO_TIMER       *timer  = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *fila   = al_create_event_queue();
    ALLEGRO_FONT        *fonte  = al_create_builtin_font();
    if (!janela || !timer || !fila || !fonte) {
        fprintf(stderr, "Falha ao criar janela, timer, fila ou fonte.\n");
        return 1;
    }
    al_set_window_title(janela, "Meu primeiro jogo em Allegro");

    /* 3. A fila recebe eventos da janela, do timer e do teclado */
    al_register_event_source(fila, al_get_display_event_source(janela));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_register_event_source(fila, al_get_keyboard_event_source());

    /* 4. Estado do jogo */
    float x = LARGURA / 2.0f, y = ALTURA / 2.0f;
    const float TAM = 40.0f, VEL = 6.0f;
    bool teclas[ALLEGRO_KEY_MAX] = { false };
    bool rodando = true, redesenhar = true;

    /* 5. Laco principal */
    al_start_timer(timer);
    while (rodando) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila, &ev);

        switch (ev.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            rodando = false;
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            teclas[ev.keyboard.keycode] = true;
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                rodando = false;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            teclas[ev.keyboard.keycode] = false;
            break;
        case ALLEGRO_EVENT_TIMER:          /* logica: 60 vezes por segundo */
            /* Alterado para WASD */
            if (teclas[ALLEGRO_KEY_A]) x -= VEL;
            if (teclas[ALLEGRO_KEY_D]) x += VEL;
            if (teclas[ALLEGRO_KEY_W]) y -= VEL;
            if (teclas[ALLEGRO_KEY_S]) y += VEL;
            
            if (x < TAM / 2) x = TAM / 2;
            if (x > LARGURA - TAM / 2) x = LARGURA - TAM / 2;
            if (y < TAM / 2) y = TAM / 2;
            if (y > ALTURA - TAM / 2) y = ALTURA - TAM / 2;
            redesenhar = true;
            break;
        }

        /* 6. Desenho */
        if (redesenhar && al_is_event_queue_empty(fila)) {
            redesenhar = false;
            /* Fundo preto (0, 0, 0) */
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            /* Quadrado branco (255, 255, 255) */
            al_draw_filled_rectangle(x - TAM / 2, y - TAM / 2, x + TAM / 2, y + TAM / 2,
                                     al_map_rgb(255, 255, 255));
                                     
            /* Atualizando o texto de instrução */
            al_draw_text(fonte, al_map_rgb(255, 255, 255), 10, 10, 0,
                         "W, A, S, D movem o quadrado. ESC para sair.");
            al_flip_display();
        }
    }

    /* 7. Liberacao dos recursos */
    al_destroy_font(fonte);
    al_destroy_event_queue(fila);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    return 0;
}