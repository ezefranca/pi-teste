#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "camera.h"
#include "otsu.h"
#include "limiarizacao.h"
#include "normalizacaorgb.h"

#define FPS 60

/******************/
struct par_ordenado{
 int pX;
 int pY;
} p1, p2, pBusca;
/******************/


void erro(char *mensagem) {
  fputs(mensagem, stderr);

  exit(EXIT_FAILURE);
}

int distancia_euclidiana(unsigned char r_velho, unsigned char g_velho, unsigned char b_velho, unsigned char r_atual, unsigned char g_atual, unsigned char b_atual){

  int soma_r = r_velho - r_atual;
  int soma_g = g_velho - g_atual;
  int soma_b = b_velho - b_atual;

  int quadrado = (soma_r * soma_r) + (soma_g * soma_g) + (soma_b * soma_b) ;
  int euclidiana = sqrt(quadrado);

  return euclidiana;
}

/******************************************************************************/
int fecho_convexo(unsigned char ***matriz, int altura, int largura){

  int i, j, k, n, a, b, c;
  int lado1, lado2;
  unsigned char x1, x2, y1, y2, pBuscaX, pBuscaY;
  
  n = altura*largura;
  //int *x = malloc(n*sizeof(unsigned char));
  //int *y = malloc(n*sizeof(unsigned char));
  // struct par_ordenado *vet_pontos = malloc(sizeof(struct par_ordenado) * (n+1));

  unsigned char ***matriz_para_convexo = camera_aloca_matriz(cam);

// Pega os pontos da tela e gera um "plano X-Y", pegando somente a primeira coordenada, ja
// que a imagem foi binarizada antes, eu poderia pegar qualquer um deles

  for (i = 0; i < altura; i++)
  {
    for (j = 0; j < largura; j++)
    {
      matriz_para_convexo[i][j][1] = matriz[i][j][1];
    }
  }
  printf("debug ----\n");
//cálculo do fecho convexo através da busca das arestas que o formam
  for(i = 0; i <= n-2; i++){
  //p1 = vet_pontos[i];
    x1 = matriz_para_convexo[i][i][1];
    y1 = matriz_para_convexo[i][i][1];
    for(j = i+1; j <= n-1; j++){
    //p2 = vet_pontos[j];
      x2 = matriz_para_convexo[i][j][1];
      y2 = matriz_para_convexo[i][j][1];

    //cálculo da equação geral da reta (aX + bY = -c)
      a = y1 - y2;
      b = x2 - x1;
      c = - (x1*y2 - y1*x2);

    //zera lados
      lado1 = 0;
      lado2 = 0;

    //em cada ponto, olha o sinal para saber de que lado da reta ele se encontra
      for(k = 0; k <= n-1; k++){
                      if((k != i) && (k != j)){ //evita de procurar no ponto extremo do segmento de reta
                       pBuscaX = x[k];
                       pBuscaY = y[k];
                           int result = a*pBuscaX + b*pBuscaY; //operação básica do algoritmo                                                    
                           if(result > c) lado1++;
                           else if(result < c) lado2++;
                           //se result = c o ponto está sobreposto no segmento de reta
                         }
                       }
                /* Para o segmento ser considerado aresta do fecho convexo,
                todos os pontos devem estar ou em lado1 ou em lado2. Se estiverem
                em ambos os lados o segmento não é aresta do fecho convexo.
                Se ambos os lados forem 0, todos os pontos estão alinhados e são
                arestas do fecho convexo. */
                if(lado1 == 0 || lado2== 0){
                 printf("%d %d\n", x1, y1);
                 printf("%d %d\n", x2, y2);
               }
                /* Melhoria que a ser feita: verificar se uma aresta não
                está contida em uma aresta maior. Mesmo assim, o algoritmo funciona
                de maneira correta */
              }
            }        
            return 0;
          }
/*********************************************************************************/

          int main() {

  //Bloco de variaveis
            int euclidiana = 0;
            float max_x = 0, max_y = 0, min_x = 0, min_y = 0;
            camera *cam = camera_inicializa(0);
  //Fim do bloco de variaveis

            if(!cam)
              erro("erro na inicializacao da camera\n");

            int largura = cam->largura;
            int altura = cam->altura;

            if(!al_init())
              erro("erro na inicializacao do allegro\n");

            if(!al_init_image_addon())
              erro("erro na inicializacao do adicional de imagem\n");

            if(!al_init_primitives_addon())
              erro("erro na inicializacao do adicional de primitivas\n");

            ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
            if(!timer)
              erro("erro na criacao do relogio\n");

            ALLEGRO_DISPLAY *display = al_create_display(2 * largura, altura);
            if(!display)
              erro("erro na criacao da janela\n");

            ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
            if(!queue)
              erro("erro na criacao da fila\n");

            al_register_event_source(queue, al_get_timer_event_source(timer));
            al_register_event_source(queue, al_get_display_event_source(display));

            al_start_timer(timer);

  /**********/

            unsigned char ***matriz = camera_aloca_matriz(cam);
            unsigned char ***fundo = camera_aloca_matriz(cam);
            unsigned char ***matriz_mao = camera_aloca_matriz(cam);

            ALLEGRO_COLOR cor = al_map_rgb_f(0, 0, 1);

            ALLEGRO_BITMAP *buffer = al_get_backbuffer(display);

            ALLEGRO_BITMAP *esquerda = al_create_sub_bitmap(buffer, 0, 0, largura, altura);

            ALLEGRO_BITMAP *direita = al_create_sub_bitmap(buffer, largura, 0, largura, altura);

  /**********/

            int desenhar = 0;
            int terminar = 0;
            int atualiza = 0;

            while(1) {

              if((atualiza%20) == 0){
                atualiza = 0;
                for (int i = 0; i < altura; i++)
                {
                  for (int j = 0; j < largura; j++)
                  {
                    for (int k = 0; k < 3; k++)
                    {
                      fundo[i][j][k] = cam->quadro[i][j][k];
                    }
                  }
                }
              }
    //normalizacaorgb(fundo, altura, largura);
              ALLEGRO_EVENT event;

              al_wait_for_event(queue, &event);

              switch(event.type) {
                case ALLEGRO_EVENT_TIMER:
                desenhar = 1;
                break;
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                terminar = 1;
                break;
                default:
                printf("evento desconhecido\n");
              }

              if(terminar)
                break;

              if(desenhar && al_is_event_queue_empty(queue)) {
                desenhar = 0;
                camera_atualiza(cam);
      /**********/

                float cy = 0;
                float cx = 0;
                int cn = 0;

      //limiarizacao(cam->quadro, altura, largura);
      //otsu_binarizacao(cam->quadro, cam->quadro, altura, largura);

                max_x = 0;
                max_y = 0;
                min_x = altura;
                min_y = largura;
                atualiza ++;

                int r, g ,b, media;

                for (int i = 0; i < altura; i++)
                {
                  for (int j = 0; j < largura; j++)
                  {
           //r = matriz[i][j][0];
           //g = matriz[i][j][1];
           //b = matriz[i][j][2];

                    euclidiana = distancia_euclidiana(fundo[i][j][0], fundo[i][j][1], fundo[i][j][2],
                      cam->quadro[i][j][0], cam->quadro[i][j][1], cam->quadro[i][j][2]);
                    if(euclidiana > 50) {




                     matriz[i][j][0] = 255;
                     matriz[i][j][1] = 255;
                     matriz[i][j][2] = 255;


                     if(r > g + b) {
                      if (i > cy && j > cx)
                      {
                        cy += i;
                        cx += j;
                        cn++;
                      }
                    }

                  }

                  else {
        // media = (r + g + b)/3;
         /*
         matriz[i][j][0] = cam->quadro[i][j][0] / media;
         matriz[i][j][1] = cam->quadro[i][j][1] / media;
         matriz[i][j][2] = cam->quadro[i][j][2] / media;
          */
         matriz[i][j][0] = 0;
         matriz[i][j][1] = 0;
         matriz[i][j][2] = 0;
       }


     }

   }



      /**********/
     //limiarizacao(fundo, altura, largura);
     //otsu_binarizacao(fundo, fundo, altura, largura);
   
   camera_copia(cam, cam->quadro, esquerda);
   if(cn > 0)
    al_draw_circle(cx / cn, cy / cn, 100, cor, 1);
  camera_copia(cam, matriz, direita);
  fecho_convexo(matriz, altura, largura);
       //camera_copia(cam, fundo, direita);
      /**********/


  al_flip_display();
}
}

  /**********/

al_destroy_bitmap(direita);

al_destroy_bitmap(esquerda);

camera_libera_matriz(cam, matriz);

  /**********/

al_stop_timer(timer);

al_unregister_event_source(queue, al_get_display_event_source(display));
al_unregister_event_source(queue, al_get_timer_event_source(timer));

al_destroy_event_queue(queue);
al_destroy_display(display);
al_destroy_timer(timer);

al_shutdown_primitives_addon();
al_shutdown_image_addon();
al_uninstall_system();

camera_finaliza(cam);

return EXIT_SUCCESS;
}
