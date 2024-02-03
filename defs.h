#include <stdio.h>

#define WHERE "/data/data/com.termux/files/home/storage/shared/"
#define FILE_NAME "xd.bmp"

#define IMAGE_LENGHT_LIMIT 5000

// Normal pixel
struct _PIXEL_N {
  uint8_t RED;
  uint8_t GREEN;
  uint8_t BLUE;
} __attribute__((packed));

// Pixel reversed
struct _PIXEL_R {
  uint8_t BLUE;
  uint8_t GREEN;
  uint8_t RED;
} __attribute__((packed));


typedef struct HEADER HEADER;
typedef struct _PIXEL_N RGB_t;
typedef struct _PIXEL_R BGR_t;

struct HEADER{
  uint8_t bm[2]; // Theres a BM here

  // Size of the file(including the header)
  uint32_t file_size;

  // This is ridiculous
  // Should be zero
  uint16_t reserved1;
  uint16_t reserved2;

  // This is where the actual bitmap
  // can be found (or the size of the
  // header, whatever you want)
  uint32_t offset;

  /*
   * The number of bytes in thee
   * header (from this point, including
   * this member).
   */
  uint32_t reoffset;

  // With and height of the image
  // (in pixels)
  uint32_t width;
  uint32_t height;

  /*
   * I honestly don't knoww what this
   * is, but it should be one for normal
   * images.
   */
  uint16_t planes;

  // Number of bits per pixel
  // Could be 1, 2, 4, 24, etc (IDK)
  uint16_t q_resolution;

  // Level of compression(for uncompressed
  // images is 0)
  uint32_t comp;

  // The size of the data after the header
  uint32_t data_size;

  // Resolution of the image
  // (IDK what this is)
  uint32_t h_res;
  uint32_t v_res;

  // Number of colors in the pallete
  uint32_t pallete_n;

  // Important colors (IDK
  // how does this work)
  uint32_t imp;


  // Here is the 1-bit color table
  BGR_t Color1;
  uint8_t reserved_table1;
  BGR_t Color2;
  uint8_t reserved_table2;

  // Now comes the real bitmap
  // The bitmap must have a 4-padding
} __attribute__((packed));

/*
struct Bit_Field {
  unsigned b1 : 1;
  unsigned b2 : 1;
  unsigned b3 : 1;
  unsigned b4 : 1;
  unsigned b5 : 1;
  unsigned b6 : 1;
  unsigned b7 : 1;
  unsigned b8 : 1;
}
*/void prueba(void);
void read_header(FILE *fp);
void write_hchess(FILE *fp, uint32_t width,
    uint32_t height, BGR_t Colors[2], uint32_t res);

void print_pixel(BGR_t Color);
void aux_print_row(unsigned char *row, uint32_t len);

static inline void arr_to_bmp(FILE *fp, const uint16_t height,
                const uint16_t width, const char *map,
                const uint16_t res);

void langton(const uint16_t lim_x, const uint16_t lim_y,
             const uint16_t res, const uint32_t lim_steps,
             const uint8_t fps, const uint16_t steps_jmp,
             const uint32_t lim_fps_vid);

