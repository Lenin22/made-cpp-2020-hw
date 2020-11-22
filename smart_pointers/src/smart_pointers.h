#pragma once


namespace task {

template <class T>
class UniquePtr {
 public:
  UniquePtr(T* raw) : pointer(raw) {}
  UniquePtr(UniquePtr&& other);
  UniquePtr(const UniquePtr& ptr_) = delete;
  ~UniquePtr();
  UniquePtr& operator=(UniquePtr&& other);
  UniquePtr& operator=(const UniquePtr& other) = delete;
  T& operator*() const;
  T* operator->() const;
  T* get() const;
  T* release();
  void reset(T * ptr);
  void swap(UniquePtr& other);
 private:
  T* pointer;
};

template <class T>
class WeakPtr;

template <class T>
class SharedPtr {
 public:
  SharedPtr();
  SharedPtr(T* ptr);
  SharedPtr(const SharedPtr& other);
  SharedPtr(SharedPtr&& other);
  SharedPtr(const WeakPtr<T>& other);
  ~SharedPtr();
  SharedPtr& operator=(SharedPtr&& other);
  SharedPtr& operator=(const SharedPtr& other);
  T& operator*() const;
  T* operator->() const;
  T* get() const;
  std::size_t use_count() const;
  std::size_t* get_counter() const { return counter; }
  void reset(T * ptr);
  void reset();
  void swap(SharedPtr& other);
 private:
  std::size_t* counter;
  T* pointer;
};

template <class T>
class WeakPtr {
 public:
  WeakPtr();
  WeakPtr(const SharedPtr<T>& other);
  WeakPtr(const WeakPtr& other);
  WeakPtr(WeakPtr&& other);
  WeakPtr& operator=(const WeakPtr& other);
  WeakPtr& operator=(WeakPtr&& other);
  WeakPtr& operator=(const SharedPtr<T>& other);
  ~WeakPtr();
  std::size_t use_count() const;
  bool expired() const;
  SharedPtr<T> lock() const;
  void reset();
  void swap(WeakPtr& other);
  T* get() const { return pointer; }
  std::size_t* get_counter() const { return counter; }
 private:
  T* pointer;
  std::size_t* counter;
};

}  // namespace task


#include "smart_pointers.tpp"
