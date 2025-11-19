#ifndef PARALEL_H
#define PARALEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// INVERTIR IMAGEN B/W PARALLEL
void inv_img_bw_p(const char mask[10], const char *path){
  //FILE *fptr;
  FILE *image, *outputImage;
  //char add_char[100] = "./img/";
  char add_char[100] = "./imagenes_act3/";
  strcat(add_char, mask);
  strcat(add_char, ".bmp");
  printf("%s\n", add_char);
  
  // char data1[80] = "./img/arc1.txt";
  // fptr = fopen(data1, "a");
  long ancho, alto, tam;
  
  image = fopen(path,"rb");
  outputImage = fopen(add_char,"wb");
  unsigned char xx[54];
  // Leer y copiar header
  for(int i=0; i<54; i++){
    xx[i] = fgetc(image);
    fputc(xx[i], outputImage);
  }
  
  tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
  ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
  alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
  
  printf("----- Inicio INV B/W •P -----\n");
  // fprintf(fptr, "%s\n", add_char);
  // fprintf(fptr, "Tamaño del archivo = %li\n", tam);
  // fprintf(fptr, "Ancho de la imagen = %li\n", ancho);
  // fprintf(fptr, "Alto de la imagen = %li\n", alto);
  
  long total_pixels = ancho * alto;
  
  unsigned char* arr_in_rgb = (unsigned char*)malloc(total_pixels * 3 * sizeof(unsigned char));
  unsigned char* arr_out_bw = (unsigned char*)malloc(total_pixels * 3 * sizeof(unsigned char));
  fread(arr_in_rgb, sizeof(unsigned char), total_pixels * 3, image);
  
  #pragma omp parallel for
  for(long i = 0; i < total_pixels; i++){
    long idx_in = i * 3;
    unsigned char b = arr_in_rgb[idx_in];
    unsigned char g = arr_in_rgb[idx_in + 1];
    unsigned char r = arr_in_rgb[idx_in + 2];
    
    // Convertir a escala de grises
    unsigned char gray = 0.21*r + 0.72*g + 0.07*b;
    
    // Calcular posición invertida
    long idx_out = ((total_pixels - 1) - i) * 3;
    
    arr_out_bw[idx_out]     = gray;  // B
    arr_out_bw[idx_out + 1] = gray;  // G
    arr_out_bw[idx_out + 2] = gray;  // R
  }
  
  fwrite(arr_out_bw, sizeof(unsigned char), total_pixels * 3, outputImage);
  
  printf("tamaño archivo %li\n", tam);
  printf("largo img %li\n", alto);
  printf("ancho img %li\n", ancho);
  
  free(arr_in_rgb);
  free(arr_out_bw);
  fclose(image);
  fclose(outputImage);
  //fclose(fptr);
  printf("Imagen B/W guardada exitosamente\n");
}

// REFLECT IMAGEN B/W PARALLEL
void reflect_img_bw_p(const char mask[10], const char *path){
  //FILE *fptr;
  FILE *image, *outputImage;
  // char add_char[100] = "./img/";
  char add_char[100] = "./imagenes_act3/";
  strcat(add_char, mask);
  strcat(add_char, ".bmp");
  printf("%s\n", add_char);
  
  // char data1[80] = "./img/arc1.txt";
  // fptr = fopen(data1, "a");
  long ancho, alto, tam;
  
  image = fopen(path,"rb");
  if(!image){
    printf("Error al abrir imagen original\n");
    return;
  }
  
  outputImage = fopen(add_char,"wb");
  if(!outputImage){
    printf("Error al crear imagen de salida\n");
    fclose(image);
    return;
  }
  
  unsigned char xx[54];
  
  // Leer y copiar header
  for(int i=0; i<54; i++){
    xx[i] = fgetc(image);
    fputc(xx[i], outputImage);
  }
  
  tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
  ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
  alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
  
  printf("----- Inicio REFLECT B/W •P -----\n");
  // fprintf(fptr, "%s\n", add_char);
  // fprintf(fptr, "Tamaño del archivo = %li\n", tam);
  // fprintf(fptr, "Ancho de la imagen = %li\n", ancho);
  // fprintf(fptr, "Alto de la imagen = %li\n", alto);
  
  long total_pixels = ancho * alto;
  
  unsigned char* arr_in_rgb = (unsigned char*)malloc(total_pixels * 3 * sizeof(unsigned char));
  unsigned char* arr_out_bw = (unsigned char*)malloc(total_pixels * 3 * sizeof(unsigned char));
  
  if(!arr_in_rgb || !arr_out_bw){
    printf("Error al asignar memoria\n");
    fclose(image);
    fclose(outputImage);
    //fclose(fptr);
    return;
  }
  
  fread(arr_in_rgb, sizeof(unsigned char), total_pixels * 3, image);
  
  #pragma omp parallel for
  for(long y = 0; y < alto; y++){
    for(long x = 0; x < ancho; x++){
      long idx_in = (y * ancho + x) * 3;
      
      // Leer pixel BGR
      unsigned char b = arr_in_rgb[idx_in];
      unsigned char g = arr_in_rgb[idx_in + 1];
      unsigned char r = arr_in_rgb[idx_in + 2];
      
      // Convertir a escala de grises
      unsigned char gray = 0.21*r + 0.72*g + 0.07*b;
      
      // Calcular índice reflejado horizontalmente
      long idx_out = (y * ancho + (ancho - 1 - x)) * 3;
      
      arr_out_bw[idx_out]     = gray;  // B
      arr_out_bw[idx_out + 1] = gray;  // G
      arr_out_bw[idx_out + 2] = gray;  // R
    }
  }
  
  fwrite(arr_out_bw, sizeof(unsigned char), total_pixels * 3, outputImage);
  
  printf("tamaño archivo %li\n", tam);
  printf("largo img %li\n", alto);
  printf("ancho img %li\n", ancho);
  
  free(arr_in_rgb);
  free(arr_out_bw);
  fclose(image);
  fclose(outputImage);
  // fclose(fptr);
  printf("Imagen B/W reflejada guardada exitosamente\n");
}

// INVERTIR IMAGEN COLOR PARALLEL (FUNCIÓN SIN USAR)
/*
void inv_img_color_p(const char mask[10], const char *path){
  FILE *image, *outputImage;
  //char add_char[100] = "./img/";
  char add_char[100] = "./imagenes_act3/";
  strcat(add_char, mask);
  strcat(add_char, ".bmp");
  long ancho, alto, tam;
  unsigned char r, g, b;
  
  image = fopen(path,"rb");
  outputImage = fopen(add_char,"wb");
  unsigned char xx[54];
  
  // Copiar header (secuencial)
  for(int i=0; i<54; i++) {
    xx[i] = fgetc(image);
    fputc(xx[i], outputImage);
  }
  
  tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
  ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
  alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];

  printf("----- Inicio INV CLR •P -----\n");
  
  printf("tamaño archivo %li\n", tam);
  // printf("bits por pixel %li\n", bpp);
  printf("largo img %li\n", alto);
  printf("ancho img %li\n", ancho);
  
  long total_pixels = ancho * alto;
  
  // Buffers de entrada
  unsigned char* arr_in_b = (unsigned char*)malloc(total_pixels * sizeof(unsigned char));
  unsigned char* arr_in_g = (unsigned char*)malloc(total_pixels * sizeof(unsigned char));
  unsigned char* arr_in_r = (unsigned char*)malloc(total_pixels * sizeof(unsigned char));
  
  // Buffers de salida
  unsigned char* arr_out_b = (unsigned char*)malloc(total_pixels * sizeof(unsigned char));
  unsigned char* arr_out_g = (unsigned char*)malloc(total_pixels * sizeof(unsigned char));
  unsigned char* arr_out_r = (unsigned char*)malloc(total_pixels * sizeof(unsigned char));

  // Lectura secuencial
  for(long j = 0; j < total_pixels; j++){
    arr_in_b[j] = fgetc(image);
    arr_in_g[j] = fgetc(image);
    arr_in_r[j] = fgetc(image);
  }
  
  #pragma omp parallel for
  for(long i = 0; i < total_pixels; i++){
    long idx = (total_pixels - 1) - i;
    arr_out_b[i] = arr_in_b[idx];
    arr_out_g[i] = arr_in_g[idx];
    arr_out_r[i] = arr_in_r[idx];
  }
  
  for(long i = 0; i < total_pixels; i++){
    fputc(arr_out_b[i], outputImage);
    fputc(arr_out_g[i], outputImage);
    fputc(arr_out_r[i], outputImage);
  }
  
  free(arr_in_b); free(arr_in_g); free(arr_in_r);
  free(arr_out_b); free(arr_out_g); free(arr_out_r);
  fclose(image);
  fclose(outputImage);
}
*/

// REFLECT IMAGEN COLOR PARALLEL
void reflect_img_color_p(const char mask[10], const char *path){
  // FILE *fptr;
  FILE *image, *outputImage;
  //char add_char[100] = "./img/";
  char add_char[100] = "./imagenes_act3/";
  strcat(add_char, mask);
  strcat(add_char, ".bmp");
  printf("%s\n", add_char);
  
  // char data1[80] = "./img/arc1.txt";
  // fptr = fopen(data1, "a");
  
  long ancho, tam, bpp, alto;
  
  image = fopen(path,"rb");
  if(!image){
    printf("Error al abrir imagen original\n");
    return;
  }
  
  outputImage = fopen(add_char,"wb");
  if(!outputImage){
    printf("Error al crear imagen de salida\n");
    fclose(image);
    return;
  }
  
  unsigned char xx[54];
  
  // Copiar header
  for(int i=0; i<54; i++) {
    xx[i] = fgetc(image);
    fputc(xx[i], outputImage);
  }
  
  tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
  bpp = (long)xx[29]*256+(long)xx[28];
  ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
  alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
  
  printf("----- Inicio REFLECT COLOR •P -----\n");
  // fprintf(fptr, "%s\n", add_char);
  // fprintf(fptr, "Tamaño del archivo = %li\n", tam);
  // fprintf(fptr, "Ancho de la imagen = %li\n", ancho);
  // fprintf(fptr, "Alto de la imagen = %li\n", alto);
  
  printf("tamaño archivo %li\n", tam);
  printf("largo img %li\n", alto);
  printf("ancho img %li\n", ancho);
  
  long total_pixels = ancho * alto;
  
  unsigned char* arr_in = (unsigned char*)malloc(total_pixels * 3 * sizeof(unsigned char));
  unsigned char* arr_out = (unsigned char*)malloc(total_pixels * 3 * sizeof(unsigned char));
  
  if (!arr_in || !arr_out) {
    printf("Error al asignar memoria\n");
    free(arr_in);
    free(arr_out);
    fclose(image);
    fclose(outputImage);
    //fclose(fptr);
    return;
  }
  
  fread(arr_in, sizeof(unsigned char), total_pixels * 3, image);
  
  #pragma omp parallel for
  for(long y = 0; y < alto; y++){
    for(long x = 0; x < ancho; x++){
      long idx_in = (y * ancho + x) * 3;
      long idx_out = (y * ancho + (ancho - 1 - x)) * 3;
      arr_out[idx_out]     = arr_in[idx_in];      // B
      arr_out[idx_out + 1] = arr_in[idx_in + 1];  // G
      arr_out[idx_out + 2] = arr_in[idx_in + 2];  // R
    }
  }
  
  fwrite(arr_out, sizeof(unsigned char), total_pixels * 3, outputImage);
  
  free(arr_in);
  free(arr_out);
  fclose(image);
  fclose(outputImage);
  // fclose(fptr);
  printf("Imagen color reflejada guardada exitosamente\n");
}


// BLUR IMAGEN PARALELIZADO
void blurP(int mm, const char mask[10], const char *path)
{
    // FILE *fptr;
    FILE *image = fopen(path, "rb");
    //char add_char[80] = "./img/";
    char add_char[100] = "./imagenes_act3/";
    char mma[20];
    // char data1[80] = "./img/arc1.txt";
    // fptr = fopen(data1, "a"); 
    strcat(add_char, mask);
    sprintf(mma, "%d", mm);
    strcat(add_char, mma);
    strcat(add_char, ".bmp");
    FILE *outputImage = fopen(add_char, "wb");

    long ancho, alto, tam;
    int j;

    unsigned char xx[54];
    for(int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    tam = (long)xx[4]*65536+(long)xx[3]*256+(long)xx[2];
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    printf("----- Inicio de bluring P -----\n" );
    printf("tamaño archivo %li\n",tam);
    printf("largo img %li\n",alto);
    printf("ancho img %li\n",ancho);
    char xx2[10] = "";
    // fprintf(fptr, "%s\n", add_char);
    // sprintf(xx2, "%li", tam);
    // fprintf(fptr, "Tamaño del archivo = ");
    // fprintf(fptr, "%s\n", xx2);
    // sprintf(xx2, "%li", ancho);
    // fprintf(fptr, "Ancho de la imagen = ");
    // fprintf(fptr, "%s\n", xx2);
    // sprintf(xx2, "%li", alto);
    // fprintf(fptr, "Alto de la imagen = ");
    // fprintf(fptr, "%s\n", xx2);
    // Extract pixel array so we can manipulate it and then write it to the output image
    int widthWithPadding = (ancho * 3 + 3) & ~3; 
  unsigned char *pixelData = malloc(alto * widthWithPadding);
    j = 0;
    int c;
    while((c = fgetc(image)) != EOF && j < ancho*alto*3){  
        pixelData[j] = c;
        j++;
    }
    printf("Inicia procesamiento\n");
    
    int kernelRadius = (mm - 1) / 2;

    // int widthWithPadding = (ancho * 3 + 3) & -4;
    // printf("padding %d\n", widthWithPadding);
    // printf("...\n");
    float progreso;
    #pragma omp parallel
{
    #pragma omp for
    for (int y = 0; y < alto; y++)
    {
        if (y % 100 == 0) {
            float progreso = ((float)y/(float)alto)*100.0;  // ✓ Variable local
            #pragma omp critical
            {
                printf("\x1b[1F\r Avance %0.2f / 100 \n", progreso);
            }
        }
        for (int x = 0; x < ancho; x++)
        {
            unsigned int rSum = 0;
            unsigned int gSum = 0;
            unsigned int bSum = 0;

            int pond = 0;

            for (int ky = -kernelRadius; ky <= kernelRadius; ky++)
            {
                for (int kx = -kernelRadius; kx <= kernelRadius; kx++)
                {
                    // Kernel sliding along the pixel at (x,y)
                    int i = y + ky;
                    int j = x + kx;

                    // Check if the kernel cordinate (i,j) is within image boundries
                    // If it isn't, we just don't consider it for the (x,y) pixel's wighted sum
                    if (i < 0 || i >= alto || j < 0 || j >= ancho)
                        continue;

                    // Each row of pixels contains its corresponding padding, so we need
                    // to use the width accounting for padding instead of just the width
                    int index = (i * widthWithPadding) + (j * 3);
                    bSum += pixelData[index];
                    gSum += pixelData[index + 1];
                    rSum += pixelData[index + 2];
                    pond++;
                }
            }

            unsigned char bAvg = bSum / pond;
            unsigned char gAvg = gSum / pond;
            unsigned char rAvg = rSum / pond;

            int index = (y * widthWithPadding) + (x * 3);
            pixelData[index] = bAvg;
            pixelData[index + 1] = gAvg;
            pixelData[index + 2] = rAvg;
        }
    
    }
}
    printf("\x1b[1F");
    printf("\r Avance 100 / 100 \n");
    printf("Fin procesamiento\n");
    for(int i = 0; i < ancho*alto*3; i++){
        fputc(pixelData[i], outputImage);
    }
    fclose(image);
    fclose(outputImage);

    free(pixelData);
}

#endif