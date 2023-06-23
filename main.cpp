#include "serial.hpp"

int main(){
    Serial::Serial serial("/dev/ttyACM0");
    char buffer[1];

    int i = 0;
    while (i < 500) {
        serial.readData();
        i++;
    }
    return 0;
}