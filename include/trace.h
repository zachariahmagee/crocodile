#include <vector>
#include <cstddef>
#include <stdexcept>

class Trace {
private:
  size_t head, tail;
  size_t capacity;
  size_t window;
  size_t size;

public:
  char *label;
  std::vector<double> data;

  Trace(size_t capacity) : head(0), tail(0), size(0), window(500), capacity(capacity), data(capacity, 0.0) {
    if (capacity <= 0) {
      throw std::invalid_argument("Capacity must be greater than 0.");
    } 
  }

  Trace(size_t capacity, size_t window): head(0), tail(0), size(0), window(window), capacity(capacity), data(capacity, 0.0) {
    if (capacity <= 0) {
      throw std::invalid_argument("Capacity must be greater than 0.");
    }
  }

  void push(double value) {
    data[head] = value;
    head = (head + 1) % capacity;

    if (size == window) {
      tail = (tail + 1) % capacity;
    } else {
      size++;
    }
  }
  
  // TODO: figure out the instance when there is < window number of points.
  double at(size_t index) const {
    if (index >= window) {
      throw std::out_of_range("Index exceeds window size.");
    }
    
    size_t actual_index = (tail + index) % capacity;
    return data[actual_index];
  }

  double operator[](size_t index) const {
    return at(index);
  }

  double at_actual(size_t index) const {
    if (index <= 0 || index >= data.size()) {
      throw std::out_of_range("Index exceeds capacity.");
    }
    return data[index];
  }

  void set_window(size_t new_window) {
    window = new_window; 
    if (head - new_window > 0) {
      tail = head - new_window;
      size = new_window;
    }
  }

  size_t get_size() {
    return size;
  }

  void setLabel(char* chars) {
    label = chars;
  }
  
  // TODO: finish this function
  // std::vector<double> get_window() const {
  //
  //   std::vector<double> result(size);
  // }

};
