#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <errno.h>
#include "modbus.h"
#include <assert.h>
#include <errno.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// Default port name
// You can specify when launching program
// eg. ./test_rtu /dev/ttyUSB1
#define PORT_NAME "/dev/ttyUSB0"


// Idk why this isn't provided in the datasheet
#define REG_PRESSURE 0x01 
#define REG_OUT1 0x10 
#define REG_OUT1_UPPERLIM 0x11 
#define REG_OUT1_LOWERLIM 0x12 
#define REG_OUT1_MODE 0x13 
#define REG_DISP_COLOR 0x14
#define REG_DISP_UNITS 0x15
#define REG_DISP_NONC 0x16 


int16_t parse_bytes_uint16(uint16_t a, uint16_t b);
int read_pressure(modbus_t *ctx, int device_addr, int reg_addr);
int read_pressure_value(modbus_t *ctx, int device_addr, int reg_addr);
