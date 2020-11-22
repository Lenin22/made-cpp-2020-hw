namespace task {

template<class T>
UniquePtr<T>::UniquePtr(UniquePtr && other) {
  pointer = other.pointer;
  other.pointer = nullptr;
}

template<class T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& other) {
  if (other.pointer != pointer) {
    delete pointer;
    pointer = other.pointer;
  }
  other.pointer = nullptr;
  return *this;
}

template<class T>
T& UniquePtr<T>::operator*() const {
  return *pointer;
}

template<class T>
T* UniquePtr<T>::operator->() const {
  return pointer;
}

template<class T>
T* UniquePtr<T>::get() const {
  return pointer;
}

template<class T>
void UniquePtr<T>::swap(UniquePtr & other) {
  if (this != &other) {
    std::swap(other.pointer, pointer);
  }
}

template<class T>
T* UniquePtr<T>::release() {
  T* temp = pointer;
  pointer = nullptr;
  return temp;
}

template<class T>
void UniquePtr<T>::reset(T * ptr) {
  if (ptr != pointer) {
    delete pointer;
    pointer = ptr;
  }
}

template<class T>
UniquePtr<T>::~UniquePtr() {
  delete pointer;
  pointer = nullptr;
}

template<class T>
SharedPtr<T>::SharedPtr() {
  counter = new std::size_t(1);
  pointer = nullptr;
}

template<class T>
SharedPtr<T>::SharedPtr(T* ptr) {
  pointer = ptr;
  counter = new std::size_t(1);
}

template<class T>
SharedPtr<T>::SharedPtr(const SharedPtr & other) {
  counter = other.counter;
  pointer = other.pointer;
  ++*counter;
}

template<class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& other) : counter(other.get_counter()),
                                                   pointer(other.get()) {
  if (counter != nullptr) {
      ++*counter;
  }
}

template<class T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) {
  pointer = other.pointer;
  counter = other.counter;
  other.counter = nullptr;
  other.pointer = nullptr;
}

template<class T>
SharedPtr<T>::~SharedPtr() {
  if (use_count() <= 1) {
    delete pointer;
    if (counter != nullptr) {
      *counter = 0;
    }
  } else {
    --*counter;
  }
}

template<class T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) {
  if (counter != nullptr)
    --*counter;
  pointer = other.pointer;
  counter = other.counter;
  other.pointer = nullptr;
  other.counter = nullptr;
  ++*counter;
  return *this;
}

template<class T>
T& SharedPtr<T>::operator*() const {
  return *pointer;
}

template<class T>
T* SharedPtr<T>::operator->() const {
  return pointer;
}

template<class T>
T* SharedPtr<T>::get() const {
  return pointer;
}

template<class T>
std::size_t SharedPtr<T>::use_count() const {
  if (counter == nullptr) {
    return 0;
  }
  return *counter;
}

template<class T>
void SharedPtr<T>::reset(T* ptr) {
  if (ptr != pointer) {
    if (*counter > 1) {
      --*counter;
    } else {
      delete counter;
      delete pointer;
    }
    pointer = ptr;
  }
}

template<class T>
void SharedPtr<T>::reset() {
  SharedPtr<T>().swap(*this);
}

template<class T>
void SharedPtr<T>::swap(SharedPtr& other) {
  if (this != &other) {
    std::swap(other.pointer, pointer);
    std::swap(other.counter, counter);
  }
}

template<class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other) {
  if (this != &other) {
    reset();
    counter = other.counter;
    pointer = other.pointer;
    ++*counter;
  }
  return *this;
}


template<class T>
WeakPtr<T>::WeakPtr() : pointer(nullptr), counter(nullptr) {}

template<class T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& other) : pointer(other.get()),
                                                 counter(other.get_counter()) {}

template<class T>
WeakPtr<T>::WeakPtr(const WeakPtr & other) : pointer(other.get()),
                                             counter(other.get_counter()) {}

template<class T>
WeakPtr<T>::WeakPtr(WeakPtr && other) : pointer(other.pointer),
                                        counter(other.counter) {
  other.counter = nullptr;
  other.pointer = nullptr;
}

template<class T>
WeakPtr<T> & WeakPtr<T>::operator=(const WeakPtr & other) {
  WeakPtr{ other }.swap(*this);
  return *this;
}

template<class T>
WeakPtr<T> & WeakPtr<T>::operator=(WeakPtr && other) {
  WeakPtr{ other }.swap(*this);
  other.pointer = nullptr;
  other.counter = nullptr;
  return *this;
}

template<class T>
WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<T>& other) {
  pointer = other.get();
  counter = other.get_counter();
  return *this;
}

template<class T>
WeakPtr<T>::~WeakPtr() {
}

template<class T>
std::size_t WeakPtr<T>::use_count() const {
  return expired() ? 0 : *counter;
}

template<class T>
bool WeakPtr<T>::expired() const {
  if (counter) {
    return *counter < 1;
  }
  return true;
}

template<class T>
SharedPtr<T> WeakPtr<T>::lock() const {
  if (counter == nullptr) {
    return SharedPtr<T>();
  }
  return SharedPtr<T>(*this);
}

template<class T>
void WeakPtr<T>::reset() {
  pointer = nullptr;
  counter = nullptr;
}

template<class T>
void WeakPtr<T>::swap(WeakPtr & other) {
  std::swap(pointer, other.pointer);
  std::swap(counter, other.counter);
}

}  // namespace task
