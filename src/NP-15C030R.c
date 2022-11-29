#include "NP-15C030R.h"

int read_pressure(modbus_t *ctx, int device_addr, int reg_addr){
    uint16_t table[2] = {0,0};
    int value;
    int ret, nb;
    nb = 1;
    modbus_set_slave(ctx, device_addr);
    ret = modbus_read_input_registers(ctx, reg_addr, nb, table);
    value = parse_bytes_uint16(table[1], table[0]);     
    if(ret != -1)
        printf("read success : register addr: %d, size = %d byte, value = %d \n", reg_addr, nb, value);
    else
    {
        printf("read error: %s\n", modbus_strerror(errno));
        return ret;
    }
    return value;
}

int read_pressure_value(modbus_t *ctx, int device_addr, int reg_addr){
    uint16_t table[2] = {0,0};
    int value;
    int ret, nb;
    nb = 1;
    modbus_set_slave(ctx, device_addr);
    ret = modbus_read_registers(ctx, reg_addr, nb, table);
    value = parse_bytes_uint16(table[1], table[0]);     
    if(ret != -1)
        printf("read success : register addr: %d, size = %d byte, value = %d \n", reg_addr, nb, value);
    else
    {
        printf("read error: %s\n", modbus_strerror(errno));
        return ret;
    }
    return value;
}

int16_t parse_bytes_uint16(uint16_t a, uint16_t b){
  uint16_t temp;
  int16_t ret;
  temp = ((uint16_t) a) << 8 | b;
  memcpy (&ret, &temp, sizeof(int16_t));
  return ret;
}
