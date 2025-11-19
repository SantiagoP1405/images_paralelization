#include <stdio.h>
#include <stdlib.h>
#include "selec_invertion_paralel.h"
#include <omp.h>
#define NUM_THREADS 30

void iteracion(const char *imagen);

int main(){  
  omp_set_num_threads(NUM_THREADS);
  //char add_char[100] = "";
  double t1, t2, tiempo; 
  t1 = omp_get_wtime();
  #pragma omp parallel
{
     #pragma omp sections
    {
        #pragma omp section //1
         (void)iteracion("imagen1");
        #pragma omp section //2
         (void)iteracion("imagen2");
        #pragma omp section //3
         (void)iteracion("imagen3");
        #pragma omp section //4
         (void)iteracion("imagen4");
        #pragma omp section //5
         (void)iteracion("imagen5");
        #pragma omp section //6
         (void)iteracion("imagen6");
        #pragma omp section //7
         (void)iteracion("imagen7");
        #pragma omp section //8
         (void)iteracion("imagen8");
        #pragma omp section //9
         (void)iteracion("imagen9");
        #pragma omp section //10
         (void)iteracion("imagen10");
        #pragma omp section //11
         (void)iteracion("imagen11");
        #pragma omp section //12
         (void)iteracion("imagen12");
        #pragma omp section //13
         (void)iteracion("imagen13");
        #pragma omp section //14
         (void)iteracion("imagen14");
        #pragma omp section //15
         (void)iteracion("imagen15");
        #pragma omp section //16
         (void)iteracion("imagen16");
        #pragma omp section //17
         (void)iteracion("imagen17");
        #pragma omp section //18
         (void)iteracion("imagen18");
        #pragma omp section //19
         (void)iteracion("imagen19");
        #pragma omp section //20
         (void)iteracion("imagen20");
        #pragma omp section //21
         (void)iteracion("imagen21");
        #pragma omp section //22
         (void)iteracion("imagen22");
        #pragma omp section //23
         (void)iteracion("imagen23");
        #pragma omp section //24
         (void)iteracion("imagen24");
        #pragma omp section //25
         (void)iteracion("imagen25");
        #pragma omp section //26
         (void)iteracion("imagen26");
        #pragma omp section //27
         (void)iteracion("imagen27");
        #pragma omp section //28
         (void)iteracion("imagen28");
        #pragma omp section //29
         (void)iteracion("imagen29");
        #pragma omp section //30
         (void)iteracion("imagen30");
   }
}
  t2=omp_get_wtime();
  tiempo=t2-t1;
  printf("Tiempo de procesamiento: %lf segundos\n", tiempo);
  printf("\n");
  return (0);
}
void iteracion(const char *imagen){
  char img_file[80];
  snprintf(img_file, sizeof(img_file), "%s.bmp", imagen);

  // INV IMG B/W
  char mask_bw[80];
  snprintf(mask_bw, sizeof(mask_bw), "inv_bw_%s", imagen);
  inv_img_bw_p(mask_bw, img_file);
    
  // REFLECT IMG B/W
  char mask_reflect[80];
  snprintf(mask_reflect, sizeof(mask_reflect), "reflect_bw_%s", imagen);
  reflect_img_bw_p(mask_reflect, img_file);

  // REFLECT IMG COLOR
  char mask_rc[80];
  snprintf(mask_rc, sizeof(mask_rc), "reflect_c_%s", imagen);
  reflect_img_color_p(mask_rc, img_file);

  // BLUR IMG
  char mask_blur_p[80];
  snprintf(mask_blur_p, sizeof(mask_blur_p), "blur_p_%s_", imagen);
  blurP(57, mask_blur_p, img_file);
}
