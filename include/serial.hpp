#ifndef _SERIAL_
#define _SERIAL_

#include <stdio.h>
#include <string.h>
#include <iostream>
// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()


#include <stdlib.h>
#include <poll.h>
#include <termios.h>

namespace Serial{
class Serial
{
public:
    Serial(std::string dev);
    void readData();
    void writeData(uint8_t* buffer);

private:
    void disconnectPort();
    bool configurePort();
    bool openPort();

    int file_desc;
    struct pollfd pollfds[1];
    std::string dev = "/dev/ttyTHS2";
};
}

#endif