#include <cerrno>
#include <cstdio>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

// tty.usbmodem122391201

class Serial {
private:
  const char* portName;
  bool connected = false;
  std::queue<std::string> messages;
  std::mutex queue_mutex;
  std::condition_variable queue_cv;

  int serialPort;
  std::thread readerThread, processorThread;

public:
  // TODO: Move processMessage and readSerialPort to another class?
  void processMessage() {
    while (connected) {
      std::unique_lock<std::mutex> lock(queue_mutex);
      queue_cv.wait(lock, [this] { return !messages.empty() || !connected;  });
    }

  }
  void readSerialPort(int) {}

  int disconnect() {
    this->connected = false;
    queue_cv.notify_all();
    readerThread.join();
    processorThread.join();
    close(serialPort);
    return 0;
  }

  int init(const char* portName) {
    this->portName = portName;
    serialPort = open(this->portName, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialPort == -1) {
      printf("Error opening serial port: %s - %s", portName, strerror(errno));
      return 1;
    }

    struct termios tty;
    if (tcgetattr(serialPort, &tty) != 0) {
      printf("Error getting terminal attributes: %s", strerror(errno));
      close(serialPort);
      return 1;
    }
    
    // set baud rate
    cfsetispeed(&tty, B115200); // input speed
    cfsetospeed(&tty, B115200); // output speed
    
    // Configure 8N1 (8 data bits, no parity, 1 stop bit)
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit characters
    tty.c_cflag &= ~PARENB;                     // No parity
    tty.c_cflag &= ~CSTOPB;                     // 1 stop bit
    tty.c_cflag &= ~CRTSCTS;                    // No hardware flow control
    tty.c_cflag |= (CLOCAL | CREAD);            // Enable receiver, ignore modem status lines

    // Configure raw input mode
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input mode
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // No software flow control
    tty.c_oflag &= ~OPOST;                         // Raw output
    
    // Apply settings
    if (tcsetattr(serialPort, TCSANOW, &tty) != 0) {
      printf("Error getting terminal attributes: %s", strerror(errno));
      close(serialPort);
      return 1;
    }
    this->connected = true;

    std::thread readerThread(readSerialPort, serialPort);
    std::thread processorThread(processMessage);

    printf("MESSAGE: Press \"s\" to stop");

    //char temp[256];
    //ssize_t bytes_read = read(serialPort, temp, sizeof(temp)- 1);

    return 0;
  } 
};
