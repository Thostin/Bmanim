#include "defs.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

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

void do_frame(uint32_t fps_count, char *map,
              uint16_t width, uint16_t height, uint16_t res){
  char name[150];
  sprintf(name, "/data/data/com.termux/files/home/C/projects/bmanim/bmp_files/f%d.bmp", fps_count);

  FILE *fp = fopen(name, "wb");
  if(NULL == fp){
    printf("Could not create file %s\n", name);
    return;
  }

  arr_to_bmp(fp, height, width, map, res);
}

void langton(uint16_t lim_x, uint16_t lim_y, uint16_t res,
             uint32_t lim_steps, uint8_t fps, uint16_t steps_jmp,
             uint32_t lim_fps_vid){
  /*
    * lim_x: width
    * lim_y: height
    * res: resolution 
    * lim_steps: steps limit
    * fps: fps of the video
    * steps_jmp: how many steps per frame
    */
  char map[lim_y * lim_x];
  FILE *list = fopen("bmp_files/list.txt", "w");
  memset(map, 0, lim_y * lim_x);

  uint16_t p_x, p_y;
  p_x = lim_x >> 1;
  p_y = lim_y >> 1;

  uint8_t m = 3;
  uint8_t aux;

  int k;
  uint32_t steps = 0;
  uint32_t fps_count = 0;
  uint32_t total_fps_count = 0;
  //int pid;

  char ffmpeg_command[500];
  char vid_name[10];

  memset(ffmpeg_command, 0, sizeof(ffmpeg_command));

  //scanf("%[^\n]s", ffmpeg_command);
  do_frame(0, map, lim_x, lim_y, res);
  int vid_count = 0;
  while(1){
    while(fps_count <= lim_fps_vid){
      if(steps >= lim_steps)
        goto refinal;
      for(k = 0; k < steps_jmp && steps < lim_steps; k++){
        // Morgan laws
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

    sprintf(ffmpeg_command, "ffmpeg -r %d -f image2 -s 1920x1080 -i bmp_files/f%%d.bmp -vcodec libx264 -crf 1 -pix_fmt yuv420p bmp_files/%s", fps, vid_name);
    printf("ffmpeg command: \n%s\n", ffmpeg_command);
    //getchar();
    system(ffmpeg_command);
   
    fprintf(list, "file '%s'\n", vid_name);
    /*
    pid = fork();
    if (pid == 0) {
      execl("/data/data/com.termux/files/usr/bin/ffmpeg", "ffmpeg", "-r", "60", "-f", "image2", "-s", "1920x1080", "-i", "/data/data/com.termux/files/home/C/projects/bmanim/bmp_files/f%d.bmp", "-vcodec", "libx264", "-crf", "1", "-pix_fmt", "yuv420p", "/data/data/com.termux/files/home/C/projects/bmanim/bmp_files/v0.mp4", NULL);
    } else if (pid > 0) {
      wait(NULL); / wait for child 
    } else {
         it was not possible to 
         *
         * create child process, so print error message 
      perror("fork failed");
    }
  */
   // execl(ffmpeg_command);
    system("rm bmp_files/f*.bmp");
    ++vid_count;
    memset(ffmpeg_command, 0, sizeof(ffmpeg_command));

    continue;

  refinal:
    sprintf(vid_name, "v%d.mp4", vid_count);
    sprintf(ffmpeg_command, "ffmpeg -r %d -f image2 -s 1920x1080 -i bmp_files/f%%d.bmp -vcodec libx264 -crf 1 -pix_fmt yuv420p bmp_files/%s", fps, vid_name);

    printf("ffmpeg command: \n%s\n", ffmpeg_command);
    system(ffmpeg_command);
    fprintf(list, "file '%s'\n", vid_name);
    //getchar();
    //execl(ffmpeg_command);
    /*

    pid = fork();
    if (pid == 0) {
      execl("/data/data/com.termux/files/usr/bin/ffmpeg", "ffmpeg", "-r", "60", "-f", "image2", "-s", "1920x1080", "-i", "/data/data/com.termux/files/home/C/projects/bmanim/bmp_files/f%d.bmp", "-vcodec", "libx264", "-crf", "1", "-pix_fmt", "yuv420p", "/data/data/com.termux/files/home/C/projects/bmanim/bmp_files/v0.mp4", NULL);
    } else if (pid > 0) {
      wait(NULL); / wait for child 
    } else {
        / it was not possible to create 
         * child process, so print error message 
      perror("fork failed");
    }
*/
    system("rm bmp_files/f*.bmp");
    break;
  }


  printf("STEPS: %d\n", steps);
  printf("Total fps: %u\n", total_fps_count);

  /*
  FILE *fp = fopen(WHERE LFILE_NAME, "wb");
  if(NULL == fp){
    printf("Could not open %s\n", LFILE_NAME);
    return;
  }

  arr_to_bmp(fp, lim_y, lim_x, (char *)map, res);
  */

  fflush(list);
  fclose(list);

  system("cd bmp_files && ffmpeg -f concat -safe 0 -i list.txt -c copy langton.mp4");
  
}

