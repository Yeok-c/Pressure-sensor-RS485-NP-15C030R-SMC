.PHONY: all

all: test_rtu clean
# test_rtu : test_rtu.o zdxxmp.o modbus.o modbus-rtu.o modbus-data.o 
# 	g++ test_rtu.o zdxxmp.o modbus.o modbus-rtu.o modbus-data.o -o ../test_rtu
test_rtu : test_rtu.o NP-15C030R.o modbus.o modbus-rtu.o modbus-data.o 
	g++ test_rtu.o NP-15C030R.o modbus.o modbus-rtu.o modbus-data.o -o ../test_rtu


test_rtu.o : test_rtu.cpp
	g++ test_rtu.cpp -c -I.
NP-15C030R.o : NP-15C030R.c
	gcc NP-15C030R.c -c -I.
modbus.o : modbus.c
	gcc modbus.c -c -I.
modbus-rtu.o : modbus-rtu.c
	gcc modbus-rtu.c -c -I.
modbus-data.o : modbus-data.c
	gcc modbus-data.c -c -I.

clean:
	rm -rf *.o *.exe

