#include "serial.hpp"

namespace Serial{

Serial::Serial(std::string dev) {
    this->dev = dev;
    openPort();
}

bool Serial::openPort(){
    file_desc = open(dev.c_str(), O_RDWR);

    if(file_desc < 0){
        printf("Serial port %s couldn't opened\n", dev.c_str());
        return false;
    }

    if (!configurePort()) {
        disconnectPort();
        printf("Serial Port Couldn't Configure\n");
        return false;
    }

    return true;
}

void Serial::disconnectPort(){
    close(file_desc);
}

bool Serial::configurePort(){
    fcntl(file_desc, F_SETFL, 0);
    // Create new termios struct, we call it 'uart_config_' for convention
    struct termios uart_config_;

    // Read in existing settings, and handle any error
    if(tcgetattr(file_desc, &uart_config_) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return false;
    }

    uart_config_.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    uart_config_.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    uart_config_.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    uart_config_.c_cflag |= CS8; // 8 bits per byte (most common)
    uart_config_.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    uart_config_.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    uart_config_.c_lflag &= ~ICANON;
    uart_config_.c_lflag &= ~ECHO; // Disable echo
    uart_config_.c_lflag &= ~ECHOE; // Disable erasure
    uart_config_.c_lflag &= ~ECHONL; // Disable new-line echo
    uart_config_.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    uart_config_.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    uart_config_.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    uart_config_.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    uart_config_.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    uart_config_.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    uart_config_.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 115200
    cfsetispeed(&uart_config_, B115200);
    cfsetospeed(&uart_config_, B115200);


    tcflush(file_desc, TCIFLUSH);

    // Save uart_config_ settings, also checking for error
    if (tcsetattr(file_desc, TCSANOW, &uart_config_) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return false;
    }

    // poll initilaize
    pollfds[0].fd = file_desc;
    pollfds[0].events = POLLIN;

    return true;
}

void Serial::readData(){
    char buffer[255];
    int retval = poll(pollfds, 1, 100);
    if (retval <= 0) {
        return;
    }

    if (pollfds[0].revents & POLLIN) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(file_desc , buffer, sizeof(buffer));
        std::cout<<"n: "<<n<<std::endl;
        for (size_t i = 0; i < n; i++) {
            std::cout<<"value: "<<(int)(buffer[i])<<std::endl;
        }
        if (n < 0) {
            throw std::system_error(EFAULT, std::system_category());
        }
    }
}

void Serial::writeData(uint8_t* buffer){
    int written = write(file_desc, buffer, sizeof(buffer));

    if (written < 0){
        printf("Wrote %d bytes but should be %ld bytes\n", written, sizeof(buffer));
        throw std::system_error(EFAULT, std::system_category());
    }
}
}