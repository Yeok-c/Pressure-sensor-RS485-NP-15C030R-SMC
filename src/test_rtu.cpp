#include "NP-15C030R.c"
#include <iostream>

int device_addr = 1;


int main(int argc, char *argv[])
{
    int ret_status;
    uint16_t ret_uint16;
    uint32_t ret_uint32;
    float ret_float;
    modbus_t *mb;

    char port[20];
    printf("argc = %d, argv[1] = %s\n", argc, argv[1]);
    if (argc == 2)
        strcpy(port, argv[1]);
    else
        strcpy(port, PORT_NAME);
    printf("libmodbus modbu-rtu master demo: %s, 19200, N, 8, 1\n", port);


    // modbus_new_rtu(const char *device, int baud, char parity, int data_bit, int stop_bit);
    // no parity, 8 data bits, 1 stop bit/
    mb = modbus_new_rtu(port, 19200, 'N', 8, 1);

    modbus_set_debug(mb, TRUE);
    modbus_set_response_timeout(mb, 0.5, 0);

    if (mb == NULL)
    {
        modbus_free(mb);
        printf("new rtu failed: %s\n", modbus_strerror(errno));
        return 0;
    }

    ret_status = modbus_connect(mb);

    if (ret_status == -1)
    {
        modbus_close(mb);
        modbus_free(mb);
        printf("connect failed: %s\n", modbus_strerror(errno));
        printf("You may need to start terminal as root or use sudo");
        return 0;
    }

    // printf("\n Homing...\n");
    // m_home(mb, 1);

    int program_flag = 1;
    while (program_flag == 1)
    {

        std::cout << "\nEnter a command: ";
        unsigned char x{}; // define variable x to hold user input (and zero-initialize it)
        std::cin >> x;

        switch (x)
        {
        
        case '1':
            printf("Changing destination of commands sent to 1");
            device_addr=1;
            break;

        case '2':
            printf("Changing destination of commands sent to 2");
            device_addr=2;
            break;

        case '3':
            printf("Changing destination of commands sent to 3");
            device_addr=3;
            break;

        case '/':
            printf("\nREAD REG_PRESSURE from 18 : ");
            read_pressure(mb, 21, REG_PRESSURE);
            read_pressure_value(mb, 21, REG_OUT1);
            read_pressure_value(mb, 21, REG_OUT1_UPPERLIM);
            read_pressure_value(mb, 21, REG_OUT1_LOWERLIM);
            read_pressure_value(mb, 21, REG_OUT1_MODE);
            read_pressure_value(mb, 21, REG_DISP_COLOR);
            read_pressure_value(mb, 21, REG_DISP_UNITS);
            read_pressure_value(mb, 21, REG_DISP_NONC);

            

            // printf("\nREAD REG_PRESSURE from 19: ");
            // ret_uint32 = read_value(mb, 19, REG_PRESSURE);
            // printf("\nREAD REG_PRESSURE from 20 : ");
            // ret_float = read_fvalue(mb, 20, REG_PRESSURE);
            // printf("\nREAD REG_PRESSURE from 21 : ");
            // ret_float = read_fvalue(mb, 21, REG_PRESSURE);
            break;

        case 'q':
            printf("Quitting and releasing serial");
            modbus_close(mb);
            modbus_free(mb);
            program_flag = 0;
            break;
        default:
            printf("Command not found");
            break;
        }
    }

    system("pause");
    return 0;
}

