//LIC

#ifndef DATA_DOTS_EBG_H
#define DATA_DOTS_EBG_H

#define LOAD_DOTS_EBG

namespace _dots_ebg_ns
{
  typedef enum {PIXEL_DENSITY_300PPI,PIXEL_DENSITY_600PPI,PIXEL_DENSITY_1200PPI,LAST_PIXEL_DENSITY} _pixel_density;

  const unsigned short MIN_DOT_SIZE=4;
  const unsigned short MAX_DOT_SIZE=8;
}
#endif
