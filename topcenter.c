#include <stdio.h>
#include <stdint.h>

int dump_bmp(FILE *fp) {
  uint16_t bfType;
  fread(&bfType, 2, 1, fp);
  if (bfType != 0x4D42) return -1;

  uint32_t bfSize;
  fread(&bfSize, 4, 1, fp);

  //printf("bfSize = %d\n", (int)bfSize);

  uint16_t bfReserved1, bfReserved2;
  fread(&bfReserved1, 2, 1, fp);
  fread(&bfReserved2, 2, 1, fp);
  if (bfReserved1 != 0 || bfReserved2 != 0) return -1;

  uint32_t bfOffbits;
  fread(&bfOffbits, 4, 1, fp);

  //  printf("type:%04x size:%d (reserved:%d %d) offbits:%d\n",
  //         (int)bfType, (int)bfSize, (int)bfReserved1, (int)bfReserved2, (int)bfOffbits);

  uint32_t biSize;
  int32_t biWidth, biHeight;
  uint16_t biPlanes, biBitCount;
  uint32_t biCompression, biSizeImage;
  int32_t biXPelsPerMeter, biYPelsPerMeter;
  uint32_t biClrUsed, biClrImportant;
  int isTopdownDIB = 0;
  fread(&biSize, 4, 1, fp);
  fread(&biWidth, 4, 1, fp);
  fread(&biHeight, 4, 1, fp);
  if (biHeight < 0) { isTopdownDIB = 1; biHeight *= -1; }
  fread(&biPlanes, 2, 1, fp); // must be 1
  fread(&biBitCount, 2, 1, fp);
  fread(&biCompression, 4, 1, fp);
  fread(&biSizeImage, 4, 1, fp);
  fread(&biXPelsPerMeter, 4, 1, fp);
  fread(&biYPelsPerMeter, 4, 1, fp);
  fread(&biClrUsed, 4, 1, fp);
  fread(&biClrImportant, 4, 1, fp);
  /*
  printf("BITMAPINFOHEADER. size:%d width:%d height:%d planes:%d bitCount:%d compression:%d sizeImage:%d pels/m:(%d %d) clrUsed:%d clrImportant:%d\n",
         (int)biSize, (int)biWidth, (int)biHeight, (int)biPlanes, (int)biBitCount,
         (int)biCompression, (int)biSizeImage, (int)biXPelsPerMeter, (int)biYPelsPerMeter, (int)biClrUsed, (int)biClrImportant);
  /**/
  fseek(fp, bfOffbits, SEEK_SET);
  int datasize = bfSize - bfOffbits;
  // printf("%d = %d x %d x %d\n", datasize, biBitCount/8, biWidth, biHeight);

  int x, y;
  int blank = 1, from = -1, to = -1;
  for (y=0; y<biHeight; y++) {
    for (x=0; x<biWidth; x++) {
      uint32_t color;
      fread(&color, 4, 1, fp);
      //color &= 0x00ffffff;
      color &= 0x000000ff;
      if (color < 0x80) {
        blank = 0;
        if (from < 0) from = x;
      } else {
        if (from >= 0 && to < 0) to = x-1;
      }
      // printf("%c", "$#=:."[color/51]);
    }
    // printf("\n");
    if (!blank) break;
  }
  // printf("size:(%d, %d) topcenter:(%d, %d)\n", biWidth, biHeight, (from + to) / 2, y);
  printf("%d %d %d %d\n", biWidth, biHeight, (from + to) / 2, y);
  // printf("%d x %d x %d = %d\n", biHeight, biWidth, 4, biHeight*biWidth*4);

  return 0;
}

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) return 0;

  dump_bmp(fp);

  fclose(fp);

  return 0;
}
