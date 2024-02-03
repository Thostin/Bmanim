#include "defs.h"
#include <stdlib.h>

// #define LFILE_NAME "langton.bmp"
static inline void arr_to_bmp(FILE *fp, const uint16_t height,
                const uint16_t width,
                const char *map, const uint16_t res){
  HEADER head;

  head.bm[0] = 'B';
  head.bm[1] = 'M';

  uint32_t theorical_width = ((width / 8) * res);
  uint32_t act_width = theorical_width +
    ((4 - (theorical_width % 4)) % 4);

  uint32_t data_size = height * res * (act_width);

  head.file_size = sizeof(HEADER) + data_size;

  head.reserved1 = 0;
  head.reserved2 = 0;
  head.offset = sizeof(HEADER);
  head.reoffset = 40;

  head.width = width * res;
  head.height = height * res;
  head.planes = 1;
  head.q_resolution = 1;
  head.comp = 0;
  head.data_size = data_size;

  head.h_res = 0;
  head.v_res = 0;
  head.pallete_n = 2;
  head.imp = 0;

  head.Color2.RED = 255;
  head.Color2.GREEN = 165;
  head.Color2.BLUE = 117;

  head.Color1.RED = 94;
  head.Color1.GREEN = 33;
  head.Color1.BLUE = 62;

  head.reserved_table1 = 0;
  head.reserved_table2 = 0;
  fwrite(&head, sizeof(HEADER), 1, fp);

  uint32_t i, j, r;
  unsigned char row[data_size];
  unsigned char *aux_ptr = &row[0];
  unsigned char *aux_ptr2;
  memset(row, 0, sizeof(row));

  uint32_t act_j = 0;
  for(i = 0; i < height; ){
    for(j = 0; j < width; ){
      for(r = 0; r < res; ){
        aux_ptr[act_j >> 3] |= ((map[i * width + j] + 1) << (7 - (act_j & 7)));
        ++act_j;
        ++r;
      }
      ++j;
    }
    ++i;
    act_j = 0;
    aux_ptr2 = aux_ptr + act_width;
    for (r = 0; r < res - 1; r++) {
      memcpy(aux_ptr2, aux_ptr, act_width);
      aux_ptr2 += act_width;
    }
    aux_ptr = aux_ptr2;
  }

  fwrite(row, sizeof(row), 1, fp);
  fclose(fp);
}

void print_matrix(char *map, uint16_t lim_x, uint16_t lim_y){
  for(int i = 0; i < lim_y; i++){
    for(int j = 0; j < lim_x; j++){
      printf("%3hhu ", map[i * lim_x + j]);
    }
    putchar(10);
  }
}

void do_frame(const uint32_t fps_count, const char *map,
              const uint16_t width, const uint16_t height,
              const uint16_t res){
  char name[150];
  sprintf(name, "/data/data/com.termux/files/home/C/projects/bmanim/bmp_files/f%d.bmp", fps_count);

  FILE *fp = fopen(name, "wb");
  if(NULL == fp){
    printf("Could not create file %s\n", name);
    return;
  }

  arr_to_bmp(fp, height, width, map, res);
}

void langton(const uint16_t lim_x, const uint16_t lim_y,
             const uint16_t res, const uint32_t lim_steps,
             const uint8_t fps, const uint16_t steps_jmp,
             const uint32_t lim_fps_vid){
 
  char map[lim_y * lim_x];
  FILE *list = fopen("bmp_files/list.txt", "w");
  memset(map, 0, lim_y * lim_x);

  uint16_t p_x, p_y;
  p_x = lim_x >> 1;
  p_y = lim_y >> 1;

  uint8_t m = 0;
  uint8_t aux;

  int k;
  uint32_t steps = 0;
  uint32_t fps_count = 0;
  uint32_t total_fps_count = 0;

  char ffmpeg_command[500];
  char vid_name[10];

  memset(ffmpeg_command, 0, sizeof(ffmpeg_command));

  do_frame(0, map, lim_x, lim_y, res);
  int vid_count = 0;
  while(1){
    while(fps_count <= lim_fps_vid){
      if(steps >= lim_steps)
        goto refinal;
      for(k = 0; k < steps_jmp && steps < lim_steps; k++){
        if(!(p_x < lim_x && p_y < lim_y))
          goto refinal;
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
              case 0: ++p_y;
                goto exit_nested_switch;
              case 255: --p_y; 
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
              case 0: --p_y;
                goto exit_nested_switch;
              case 255: ++p_y;
                goto exit_nested_switch;
          }
        }

        exit_nested_switch:
        switch(aux){
          case 0: --m; break;
          default: ++m;
        }

        m &= 3;
        ++steps;
      }
      if(steps >= lim_steps)
        goto refinal;
      ++fps_count;
      ++total_fps_count;

      do_frame(fps_count, map, lim_x, lim_y, res);
    }
    fps_count = 0;
    sprintf(vid_name, "v%d.mp4", vid_count);

    sprintf(ffmpeg_command, "ffmpeg -hide_banner -r %d -f image2 -s %dx%d -i bmp_files/f%%d.bmp -vcodec libx264 -crf 1 -pix_fmt yuv420p bmp_files/%s", fps, lim_x * res, lim_y * res, vid_name);
    printf("Making vi n%d\n", vid_count);
    system(ffmpeg_command);
   
    fprintf(list, "file '%s'\n", vid_name);
    system("rm bmp_files/f*.bmp");
    ++vid_count;
    memset(ffmpeg_command, 0, sizeof(ffmpeg_command));

    continue;

  refinal:
    sprintf(vid_name, "v%d.mp4", vid_count);
    printf("Making final video\n");
    sprintf(ffmpeg_command, "ffmpeg -hide_banner -r %d -f image2 -s %dx%d -i bmp_files/f%%d.bmp -vcodec libx264 -crf 1 -pix_fmt yuv420p bmp_files/%s", fps, lim_x * res, lim_y * res, vid_name);

    if(!system(ffmpeg_command)){
      fprintf(list, "file '%s'\n", vid_name);
      system("rm bmp_files/f*.bmp");
    }

    break;
  }

  fflush(list);
  fclose(list);

  system("rm ~/storage/shared/langton.mp4");
  printf("Linking videos\n");

  if(vid_count > 0){
    system("cd bmp_files && ffmpeg -hide_banner -f concat -safe 0 -i list.txt -c copy ~/storage/shared/langton.mp4");
    system("rm bmp_files/v*.mp4 bmp_files/list.txt");
  } else {
    system("mv bmp_files/v0.mp4 ~/storage/shared/langton.mp4");
  }
  printf("DONE\nSTEPS: %d\n", steps);
  printf("Total fps: %u\n", total_fps_count);
}

