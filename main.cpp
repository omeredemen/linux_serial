#include "serial.hpp"

int main(){
    Serial::Serial serial("/dev/ttyACM0");
    uint8_t buffer[5];

    int i = 0;
    while (i < 500) {
        serial.readData(buffer, 5);
        i++;
        std::cout<<"n: "<<sizeof(buffer)<<std::endl;
        for (size_t i = 0; i < sizeof(buffer); i++) {
            std::cout<<"value: "<<(float)buffer[i]<<std::endl;
        }
    }
    return 0;
}