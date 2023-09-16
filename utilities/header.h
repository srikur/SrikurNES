#include "../core/common.h"


void checkHeader(FILE *rom) {
  /* Check header */
  char header[16];
  fread(header, 1, 16, rom);
  if (header[0] != 'N' || header[1] != 'E' || header[2] != 'S' || header[3] != 0x1A) {
    printf("Error: Invalid header\n");
    exit(1);
  }

  /* Check if ROM is iNES or NES 2.0 */
  if ((header[7] & 0x0C) == 0x08) {
    printf("Error: ROM is NES 2.0\n");
    exit(1);
  }

  /* Check if ROM is PAL */
  if ((header[9] & 0x01) == 0x01) {
    printf("Error: ROM is PAL\n");
    exit(1);
  }

}
