#pragma once

#include <cstddef>
#include <utility>

template <class T>
class Chunck {
 public:
  Chunck() : data(new char[size]), used(0), prev(nullptr) {}
  ~Chunck() { delete[] data; }

 public:
  const int size = 10000;
  Chunck* prev;
  size_t used;
  char* data;
};

template <class T>
class Allocator {
 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  template <typename U>
  struct rebind {
    using other = Allocator<U>;
  };

  Allocator() : counter(new size_type(1)), last(nullptr) {}

  ~Allocator() {
    if (*counter == 1) {
      while (last != nullptr) {
        Chunck<value_type>* temp = last;
        last = last->prev;
        delete temp;
      }
    } else {
      (*counter)--;
    }
  }

  Allocator(Allocator& other) {
    (*(other.counter))++;
    counter = other.counter;
    last = other.last;
  }

  Allocator& operator=(Allocator& other) {
    this->~Allocator();  // shall I call destructor memory before assigning
    (*(other.counter))++;
    counter = other.counter;
    last = other.last;
    return *this;
  }

  pointer allocate(size_type size) {
    Chunck<value_type>* iter = last;

    // look for free space
    while (iter != nullptr && iter->used + size * sizeof(value_type) > iter->size) {
      iter = iter->prev;
    }

    if (iter == nullptr) {
      Chunck<value_type>* newChunk = new Chunck<value_type>();
      newChunk->prev = last;
      last = newChunk;
      iter = last;
    }

    pointer returned = (pointer)((iter->data) + iter->used);
    iter->used += size * sizeof(value_type);
    return returned;
  }

  void deallocate(pointer address, size_type size) {
    // do nothing, free memory in destructor
  }

  void construct(pointer address, value_type value) {
    new(reinterpret_cast<void *>(address)) value_type(value);
  }

  template <class... Args>
  void construct(pointer p, Args&&... args) {
    ::new(reinterpret_cast<void *>(p)) value_type(std::forward<Args>(args)...);
  }

  void destroy(pointer ptr) {
    ptr->~T();
  }

  size_type get_counter() const {
    return *counter;
  }

  private:
  Chunck<value_type>* last;
  size_type* counter;
};
