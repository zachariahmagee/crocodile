#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/_types/_ssize_t.h>
#include <sys/fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <concepts>
// tty.usbmodem122391201

template <typename T>
concept Pushable = requires(T obj, double value) {
  { obj.push(value) };
};

class Serial {
private:
  // const char* portName;
  std::atomic<bool> connected{false};
  std::queue<std::string> messages;
  std::mutex queue_mutex;
  std::condition_variable queue_cv;

  int serialPort;
  std::thread readerThread, processorThread;

public:
  void processMessage() {
    while (connected) {
      std::unique_lock<std::mutex> lock(queue_mutex);
      queue_cv.wait(lock, [this] { return !messages.empty() || !connected; });

      while (!messages.empty()) {
        std::string message = messages.front();
        messages.pop();
        lock.unlock();

        // TODO: Parse incoming data
        if (!messages.empty()) {
          printf("Received Line: %s\n", message.c_str());
        } else {
          printf("Warning: Empty line received\n");
        }
        lock.lock();
      }
    }
  }
  void readSerialPort() {
    std::string buffer;
    // std::string line;
    char temp[1024];

    while (connected) {
      ssize_t bytesRead = read(serialPort, temp, sizeof(temp) - 1);

      if (bytesRead > 0) {
        temp[bytesRead] = '\0';
        buffer += temp;

        // check for newlines
        size_t pos = 0;
        while ((pos = buffer.find('\n')) != std::string::npos) {
          if (pos + 1 <= buffer.size()) {
            std::string line = buffer.substr(0, pos);
            //printf("Current buf: %s (%zu)\n", buffer.c_str(), buffer.size());
            buffer.erase(0, pos + 1);
            //printf("After erase: %s, (%zu)\n", buffer.c_str(), buffer.size());
            // push line into the queue
            {
              std::lock_guard<std::mutex> lock(queue_mutex);
              messages.push(line);
              printf("Pushed line to messages: \"%s\" (%zu)\n", line.c_str(),
                     messages.size());
            }

            queue_cv.notify_one();
          } else {
            printf("Unexpected buffer state, pos: %zu, buffer size: %zu\n", pos,
                   buffer.size());
          }
        }
      } else if (bytesRead == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
        printf("Error reading serial port: %s\nDisconnecting\n",
               strerror(errno));
        connected = false;
        break;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  int disconnect() {
    if (!connected) {
      return -1;
    }
    this->connected = false;
    queue_cv.notify_all();

    if (readerThread.joinable()) {
      readerThread.join();
    }
    if (processorThread.joinable()) {
      processorThread.join();
    }
    if (serialPort != -1) {
      close(serialPort);
      serialPort = -1;
    }
    return 0;
  }

  int init(const char *portName) {
    serialPort = open(portName, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialPort == -1) {
      printf("Error opening serial port: %s - %s\n", portName, strerror(errno));
      return 1;
    }

    struct termios tty;
    if (tcgetattr(serialPort, &tty) != 0) {
      printf("Error getting terminal attributes: %s\n", strerror(errno));
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
    tty.c_cflag |=
        (CLOCAL | CREAD); // Enable receiver, ignore modem status lines

    // Configure raw input mode
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input mode
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // No software flow control
    tty.c_oflag &= ~OPOST;                          // Raw output

    // Apply settings
    if (tcsetattr(serialPort, TCSANOW, &tty) != 0) {
      printf("Error getting terminal attributes: %s\n", strerror(errno));
      close(serialPort);
      return 1;
    }
    this->connected = true;

    try {
    readerThread = std::thread(&Serial::readSerialPort, this); 
    processorThread = std::thread(&Serial::processMessage, this);
    } catch (...) {
      printf("Error starting threads.\n");
      connected = false;
      close(serialPort);
      serialPort = -1;
      return 1;
    }
    printf("MESSAGE: Press \"d\" to stop\n");

    // char temp[256];
    // ssize_t bytes_read = read(serialPort, temp, sizeof(temp)- 1);

    return 0;
  }
};
