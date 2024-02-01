#include "defs.h"

int main(
  void
  //int argc, char *argv[]
)
{
  /*
  FILE *fp = fopen(WHERE FILE_NAME, "wb");
  
  read_header(fp);
  
  if(argc != 10){
    printf("Improper input\n");
    return 1;
  }
  uint32_t width, height, res;
  width = atoi(argv[1]);
  height = atoi(argv[2]);
  res = atoi(argv[3]);
  printf("sizeof(BGR_t): %lu\n", sizeof(BGR_t));

  BGR_t Colors[2];
  Colors[0].RED = atoi(argv[4]);
  Colors[0].BLUE = atoi(argv[5]);
  Colors[0].GREEN = atoi(argv[6]);

  Colors[1].RED = atoi(argv[7]);
  Colors[1].GREEN = atoi(argv[8]);
  Colors[1].BLUE = atoi(argv[9]);
  write_hchess(fp, width, height, Colors, res);
  */ 
  langton(128, 72, 15, 15000, 60, 1, 200);
  //langton(45, 59, 600);
  //prueba();
  return 0;
}
