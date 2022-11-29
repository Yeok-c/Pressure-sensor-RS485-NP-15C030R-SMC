#include "zdxxmp.h"
#define bytes_to_u16(MSB,LSB) (((unsigned int) ((unsigned char) MSB)) & 255)<<8 | (((unsigned char) LSB)&255) 


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


int write_value(modbus_t *ctx, int device_addr, int reg_addr, uint32_t value){
    uint16_t myBuffer_2;
    uint16_t *buffer_2 = &myBuffer_2;
    defAllocator_uint16(buffer_2, 2);

    uint16_t myBuffer;
    uint16_t *buffer = &myBuffer;
    defAllocator_uint16(buffer, 1);

    int ret = 0;
    int nb;
    assert(reg_addr != REG_ACC_PARAM_H);
    modbus_set_slave(ctx, device_addr);

    nb = num_bytes(reg_addr);
    if(nb == 2){
        // uses function code 16
        writeUInt32ToBufferBigEndian(value, buffer_2);
        ret = modbus_write_registers(ctx, reg_addr, nb, buffer_2);
         
    } else if (nb==1){   
        writeUInt16ToBufferBigEndian(value, buffer);
        ret = modbus_write_register(ctx, reg_addr, *buffer);
    }

    if(ret != -1)
        printf("write success : register addr: %d, size = %d byte, value = %d \n", reg_addr, nb, value);
    else
    {
        printf("write error: %s\n", modbus_strerror(errno));
    }
    return ret;
}


void print_state_message(uint16_t state){
    if(state==STATE_UNDEFINED)  printf("State code: %04d: STATE_UNDEFINED\n", STATE_UNDEFINED);        
    if(state==STATE_IDLE)       printf("State code: %04d: STATE_IDLE\n", STATE_IDLE);       
    if(state==STATE_ACCEL)      printf("State code: %04d: STATE_ACCEL\n", STATE_ACCEL);             
    if(state==STATE_CONST)      printf("State code: %04d: STATE_CONST\n", STATE_CONST);            
    if(state==STATE_DECCEL)     printf("State code: %04d: STATE_DECCEL\n", STATE_DECCEL);
    if(state==STATE_HOMING)     printf("State code: %04d: STATE_HOMING\n", STATE_HOMING);
    if(state==STATE_ERROR_RETURNING)printf("State code: %04d: STATE_ERROR_RETURNING\n", STATE_ERROR_RETURNING);
    if(state==STATE_UP_BUTTON_PRESSED)printf("State code: %04d: STATE_UP_BUTTON_PRESSED\n", STATE_UP_BUTTON_PRESSED);
    if(state==STATE_DOWN_BUTTON_PRESSED)printf("State code: %04d: STATE_DOWN_BUTTON_PRESSED\n", STATE_DOWN_BUTTON_PRESSED);
    // else{
    //     printf("State code: %04d: UNKNOWN \n", state);
    // }
}

// Private functions ---------------------------------------

int num_bytes(int reg_addr){
    int nb;
    if ((reg_addr == REG_ADDRESS) || (reg_addr == REG_MICROSTEP) || (reg_addr == REG_LOCK_MODE) || 
        (reg_addr == REG_INIT_PERIOD) || (reg_addr == REG_MAX_PERIOD) || (reg_addr == REG_LIM_SW_OFST)||
        (reg_addr == REG_CURRENT_STATE) || (reg_addr==REG_MOVE_TO_ZERO) || (reg_addr==REG_STOP_ALL)){
        nb=1;

    } else if (reg_addr==REG_BUTTON_STATES_H) {
        nb=3;
    } else {
        nb=2;
    }
    return nb;
}

/* 
    Parses register values into integer values
    Each register contains value 0-255 (2 hex numbers)
    Eg. 1 register input:
        Input: [0x03][0x20]
        Output: 800
    Eg. 2 register input:
        Input: [0x00][0x00] [0x07][0xD0]
        Output: 2000

    TODO: 
    Change to << 16 and << 8 later!
*/

int16_t parse_bytes_uint16(uint16_t a, uint16_t b){
  uint16_t temp;
  int16_t ret;
  temp = ((uint16_t) a) << 8 | b;
  memcpy (&ret, &temp, sizeof(int16_t));
  return ret;
}


uint32_t parse_bytes(uint16_t table[], int nb){
    uint32_t intNumber;
    if(nb==1){
        intNumber = table[0];
    } 
    else if(nb==2){
        intNumber = table[0]*256*256 + table[1];
    }
    return intNumber;
}
/* 
    Parses bytes into floats, always 2 register input
    Eg. Input: [0x3C][0x23][0xD7][0x0A]
        Output: 0.010000 
*/
float parse_bytes_to_float(uint16_t table[]){
    float f;
    uint16_t table_[] = {table[1], table[0]};
    memcpy(&f, &table_, sizeof(f));
    return f;
}

// uint32_t get_UInt32FromBufferBigEndian(uint8_t* buffer){
//     static uint32_t number = 0;
//     memcpy(&number, &buffer, sizeof(uint32_t));
//     return number;
// }

// uint16_t get_UInt16FromBufferBigEndian(uint8_t* buffer){#define bytes_to_u16(MSB,LSB) (((unsigned int) ((unsigned char) MSB)) & 255)<<8 | (((unsigned char) LSB)&255) 

//     static uint16_t number = 0;
//     memcpy(&number, &buffer, sizeof(uint16_t));
//     return number;
// }

// float get_FloatFromBufferBigEndian(uint8_t* buffer){
//     static float f = 0;
//     memcpy(&f, &buffer, sizeof(float));
//     return f;
// }

// Convert values to register buffers values =============================================

void writeUInt32ToBufferBigEndian(uint32_t number, uint16_t* buffer)
{
    buffer[0] = (uint16_t) ((number >> 16) & 0xff);
    buffer[1] = (uint16_t) (number);
    // printf(" Buffer reconstructed: {%04X}{%04X} \n", buffer[0], buffer[1]); 
}

void writeFloatToBufferBigEndian(float f, uint16_t* buffer)
{	
    buffer[0] = (uint16_t) ((*(unsigned int*)(&f)) >> 16);
    buffer[1] = (uint16_t) ((*(unsigned int*)(&f)) >> 0);

    // static uint32_t number;
    // memcpy(&number, &f, sizeof(f));
    // buffer[0] = (uint16_t) ((number >> 16) & 0xff);
    // buffer[1] = (uint16_t) (number);
    // printf(" Buffer reconstructed: {%04X}{%04X} \n", buffer[0], buffer[1]); 
}

void writeUInt16ToBufferBigEndian(uint16_t number, uint16_t* buffer)
{
    buffer[0] = (uint16_t) (number);
    // printf("Buffer reconstructed: {%04X} \n", buffer[0]); 
}

// Pointer allocation
void defAllocator_uint8(uint8_t *buffer, uint8_t count) {
    buffer = (uint8_t*)malloc(count);
    // printf("Allocated %d uint8_t at the heap at %p\n", count, buffer);
}

// Pointer allocation
void defAllocator_uint16(uint16_t *buffer, uint16_t count) {
    buffer = (uint16_t*)malloc(count);
    // printf("Allocated %d uint16_t at the heap at %p\n", count, buffer);
}

// Pointer allocation
void defAllocator_uint32(uint32_t *buffer, uint32_t count) {
    buffer = (uint32_t*)malloc(count);
    // printf("Allocated %d uint32_t at the heap at %p\n", count, buffer);
}

