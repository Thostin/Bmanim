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


