#include "defs.h"

void prueba(void){
  FILE *fp = fopen(WHERE FILE_NAME, "wb");
  if(NULL == fp){
    perror("fopen: ");
    return;
  }
  HEADER head;

  head.bm[0] = 'B';
  head.bm[1] = 'M';
  head.file_size = 78;
  head.reserved1 = 0;
  head.reserved2 = 0;
  head.offset = 54;
  head.reoffset = 40;

  head.width = 2;
  head.height = 3;
  head.planes = 1;
  head.q_resolution = 24;
  head.comp = 0;
  head.data_size = 24;

  head.h_res = 0;
  head.v_res = 0;
  head.pallete_n = 0;
  head.imp = 0;
  
  unsigned char arr_BM[] =
    { 0XFF, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0X00,
      0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X00, 0X00,
      0X00, 0X00, 0XFF, 0X00, 0X00, 0XFF, 0X00, 0X00
    };
  fwrite(&head, sizeof(HEADER), 1, fp);
  fwrite(arr_BM, sizeof(arr_BM), 1, fp);
}

void read_header(FILE *fp){
  if(NULL == fp){
    perror("fopen: ");
    return;
  }

  HEADER head;
  fread(&head, sizeof(HEADER), 1, fp);

  printf("bm[0]: %c\n", head.bm[0]);
  printf("bm[1]: %c\n", head.bm[1]);

  printf("file_size: %d\n", head.file_size);
  printf("reserved1: %hu\n", head.reserved1);
  printf("reserved2: %hu\n", head.reserved2);

  printf("offset: %d\n", head.offset);
  printf("reoffset: %d\n", head.reoffset);

  printf("width: %d\n", head.width);
  printf("height: %d\n", head.height);

  printf("planes: %hu\n", head.planes);
  printf("q_resolution: %hu\n", head.q_resolution);

  printf("comp: %d\n", head.comp);
  printf("data_size: %d\n", head.data_size);
  printf("h_res: %d\n", head.h_res);
  printf("v_res: %d\n", head.v_res);
  printf("pallete_n: %d\n", head.pallete_n);
  printf("imp: %d\n", head.imp);
}

void print_pixel(BGR_t Color) {
  printf("%u %u %u\n", Color.RED, Color.GREEN, Color.BLUE);
}

void aux_print_row(unsigned char row[], uint32_t len)
{
  uint32_t act_len = (len - len % 3) / 3;
  for(int i = 0; i < act_len; )
  {
    print_pixel(*(BGR_t *)(row + 3 * i));
    ++i;
  }
}

void write_hchess(FILE *fp, uint32_t width,uint32_t height,
                  BGR_t Colors[2], uint32_t res) {
  if(NULL == fp) {
    printf("NULL file reached in write_hchess\n");
    return;
  }

  if(width > IMAGE_LENGHT_LIMIT || height > IMAGE_LENGHT_LIMIT) {
    printf("TOO LARGE IMAGE AT write_hchess\n");
    return;
  }

  HEADER head;
  head.bm[0] = 'B';
  head.bm[1] = 'M';

  uint32_t aux = sizeof(BGR_t) * width * res;
  uint32_t act_width = aux + ((4 - (aux % 4)) % 4);
  uint32_t data_size = res * height * (act_width);

  unsigned char row1[act_width];
  unsigned char row2[act_width];

  BGR_t * bgr_row1 = (BGR_t *)row1;
  BGR_t * bgr_row2 = (BGR_t *)row2;

  for(int i = 0; i < width * res; )
  {
    for(int j = 0; j < res; ){
      bgr_row1[i] = Colors[0];
      bgr_row2[i] = Colors[1];

      ++i;
      if(i == width * res)
        break;
      ++j;
    }

    for(int j = 0; j < res; j++){
      bgr_row1[i] = Colors[1];
      bgr_row2[i] = Colors[0];

      ++i;
      if(i == width * res)
        break;
    }
  }

  aux_print_row(row1, act_width);
  aux_print_row(row2, act_width);

  head.file_size = sizeof(HEADER) + data_size;
  head.reserved1 = 0;
  head.reserved2 = 0;
  head.offset = sizeof(HEADER);
  head.reoffset = 40;

  head.width = width * res;
  head.height = height * res;
  head.planes = 1;
  head.q_resolution = 24;
  head.comp = 0;
  head.data_size = data_size;

  head.h_res = 0;
  head.v_res = 0;
  head.pallete_n = 0;
  head.imp = 0;

  printf("file_size: %u\n", head.file_size);
  printf("sizeof(HEADER): %lu\n", sizeof(HEADER));
  fwrite(&head, sizeof(HEADER), 1, fp);
  for(int i = 0; i < height; ) {
    for(int j = 0; j < res; j++){
      fwrite(row1, sizeof(row1), 1, fp);
    }
    if(++i == height)
      break;
    for(int j = 0; j < res; j++){
      fwrite(row2, sizeof(row2), 1, fp);
    }
    /*
    fwrite(row1, sizeof(row1), 1, fp);

    ++i;
    if(i == height)
      break;
    fwrite(row1, sizeof(row1), 1, fp);

    if(i == height)
      break;
    ++i;
    fwrite(row2, sizeof(row2), 1, fp);
    */
    ++i;
  }

  fclose(fp);
}
/*
  unsigned char arr_BM[] = 
    {0X42, 0X4D,
      0X46, 0X00, 0X00, 0X00,
      0X00, 0X00,
      0X00, 0X00,
      0X36, 0X00, 0X00, 0X00,
      0X28, 0X00, 0X00, 0X00,

      0X02, 0X00, 0X00, 0X00,
      0X02, 0X00, 0X00, 0X00,

      0X01, 0X00,
      0X18, 0X00,

      0X00, 0X00, 0X00, 0X00,
      0X10, 0X00, 0X00, 0X00,

      0X00, 0X00, 0X00, 0X00,
      0X00, 0X00, 0X00, 0X00,

      0X00, 0X00, 0X00, 0X00,
      0X00, 0X00, 0X00, 0X00,


      0X00, 0X00, 0XFF, 0XFF, 0XFF, 0XFF, 0X00, 0X00,
      0XFF, 0XFF, 0X00, 0XFF, 0X00, 0X00, 0X00, 0X00
    };
  */
