#include "defs.h"

int main(
  // void
  int argc, char *argv[]
)
{
  FILE *fp = fopen(WHERE FILE_NAME, "wb");
  /*
  read_header(fp);
  */
  if(argc != 4){
    printf("Improper input\n");
    return 1;
  }
  uint32_t width, height, res;
  width = atoi(argv[1]);
  height = atoi(argv[2]);
  res = atoi(argv[3]);
  printf("sizeof(BGR_t): %lu\n", sizeof(BGR_t));

  BGR_t Colors[2];
  Colors[0].RED = 0;
  Colors[0].BLUE = 0;
  Colors[0].GREEN = 0;

  Colors[1].RED = 0;
  Colors[1].GREEN = 255;
  Colors[1].BLUE = 0;
  write_hchess(fp, width, height, Colors, res);
  
  //prueba();
  return 0;
}
