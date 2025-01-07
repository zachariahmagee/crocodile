#include "../include/serial.h"
#include <iostream>

Serial serial;
// tty.usbserial-A50285BI
// tty.usbmodem122391201
int main(int argc, char* argv[]) {
    printf("BEGIN:\n");
    serial.init("/dev/tty.usbmodem122391201");
    printf("Serial class initialized.\n");
    std::cin.get();
    printf("Terminating program");
    serial.disconnect();
    return 0;
}
