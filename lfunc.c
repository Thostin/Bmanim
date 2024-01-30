#include "defs.h"

#define LFILE_NAME "langton.bmp"
void arr_to_bmp(FILE *fp, uint16_t height, uint16_t width,
                char *map, uint16_t res){
  HEADER head;

  head.bm[0] = 'B';
  head.bm[1] = 'M';

  uint32_t theorical_width = sizeof(BGR_t) * width * res;
  uint32_t act_width = theorical_width +
    ((4 - (theorical_width % 4)) % 4);

  uint32_t data_size = res * height * (act_width);

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

  fwrite(&head, sizeof(HEADER), 1, fp);

  unsigned char row[act_width];
  uint8_t aux;
  int p, i, j, r;

  BGR_t *bgr_row = (BGR_t *)row;
  BGR_t aux_bgr;


  uint16_t act_j = 0;
  for(i = 0; i < height; ){
    for(j = 0; j < width; ){
      aux = map[i * width + j] + 1;
      aux *= 255;

      aux_bgr.RED = aux;
      aux_bgr.GREEN = aux;
      aux_bgr.BLUE = aux;

      for(p = 0; p < res; p++){
        bgr_row[act_j] = aux_bgr;
        ++act_j;
      }
      ++j;
    }
    act_j = 0;
    for(r = 0; r < res; r++){
      fwrite(row, act_width, 1, fp);
    }
    ++i;
  }
}

void print_matrix(char *map, uint16_t lim_x, uint16_t lim_y){
  for(int i = 0; i < lim_y; i++){
    for(int j = 0; j < lim_x; j++){
      printf("%3hhu ", map[i * lim_x + j]);
    }
    putchar(10);
  }
}

void langton(uint16_t lim_x, uint16_t lim_y, uint16_t res){
  uint8_t map[lim_y * lim_x];

  memset(map, 0, lim_y * lim_x);

  uint16_t p_x, p_y;
  p_x = lim_x >> 1;
  p_y = lim_y >> 1;

  uint8_t m = 3;
  uint8_t aux;

  while(1){
    // Morgan laws
    if(!(p_x < lim_x && p_y < lim_y))
      break;
    aux = ~map[p_y * lim_x + p_x];
    map[p_y * lim_x + p_x] = aux;

    switch(m){
      case 0:
      switch(aux){
          case 0: --p_x;
            goto exit_nested_switch;
          case 255: ++p_x;
            goto exit_nested_switch;
        }
      case 1:
      switch(aux){
          case 0: --p_y;
            goto exit_nested_switch;
          case 255: ++p_y; 
            goto exit_nested_switch;
        }
      case 2:
      switch(aux){
          case 0: ++p_x; 
            goto exit_nested_switch;
          case 255: --p_x; 
            goto exit_nested_switch;
        }

      default: //case 3:
      switch(aux){
          case 0: ++p_y;
            goto exit_nested_switch;
          case 255: --p_y;
            goto exit_nested_switch;
        }
    }

    exit_nested_switch:
    switch(aux){
      case 0: --m; break;
      default: ++m;
    }

    m &= 3;
  }
  FILE *fp = fopen(WHERE LFILE_NAME, "wb");
  if(NULL == fp){
    printf("Could not open %s\n", LFILE_NAME);
    return;
  }

  arr_to_bmp(fp, lim_y, lim_x, (char *)map, res);
}

