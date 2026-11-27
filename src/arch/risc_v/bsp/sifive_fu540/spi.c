#include "spi.h"

#define QSPI2_BASE_ADDR 0x10050000
#define QSPI_REG(addr,x) (*((volatile unsigned int*)((addr) + (x))))

#define SCKDIV       0x00
#define SCKMODE      0x04
#define CSID         0x10
#define CSDEF        0x14
#define CSMODE       0x18
#define FMT          0x40
#define TXDATA       0x48
#define RXDATA       0x4C
#define TXMARK       0x50
#define RXMARK       0X54
#define FCTRL        0x60
#define FFMT         0x64
#define IE           0x70
#define IP           0x74

#define QSPI_FMT_PROTO_SINGLE 0x00
#define QSPI_FMT_PROTO_DUAL   0x01
#define QSPI_FMT_PROTO_QUAD   0x02



void init_spi(){
  QSPI_REG(QSPI2_BASE_ADDR,SCKDIV)  = 8;
  QSPI_REG(QSPI2_BASE_ADDR,SCKMODE) = 0;
  QSPI_REG(QSPI2_BASE_ADDR,FMT)     = QSPI_FMT_PROTO_SINGLE | (8 << 16); 
  QSPI_REG(QSPI2_BASE_ADDR,CSMODE)  = 0; //auto
}

