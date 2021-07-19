//
// Created by Thomas Povinelli on 7/13/21.
//

#ifndef MAXHEAP_HEAP_H
#define MAXHEAP_HEAP_H

#include <cassert>
#include <cstddef>
#include <functional>

namespace tom {

template <typename T>
static void aswap(T arr[], int a, int b) {
  assert(arr != nullptr);
  T val = std::move(arr[a]);
  arr[a] = std::move(arr[b]);
  arr[b] = std::move(val);
}

template <typename T>
using comparator_func = int (*)(T const&, T const&);

template <typename T>
class heap {
  comparator_func<T> comparator;
  std::size_t count_ = 0;
  std::size_t capacity = 0;
  T* data = nullptr;

  void heapify() {
    std::size_t index = count_ - 1;
    while (index > 0) {
      std::size_t parent = (index - 1) / 2;

      if (parent < capacity && comparator(data[index], data[parent]) > 0) {
        aswap(data, index, parent);
        index = parent;
      } else {
        break;
      }
    }
  }

  void sift_down(std::size_t idx) noexcept {
    std::size_t child1 = (idx * 2) + 1;
    std::size_t child2 = (idx * 2) + 2;

    if (child1 < count_ && comparator(data[child1], data[idx]) > 0) {
      aswap(data, child1, idx);
      sift_down(child1);
    }
    if (child2 < count_ && comparator(data[child2], data[idx]) > 0) {
      aswap(data, child2, idx);
      sift_down(child2);
    }
  }

  void resize(std::size_t target) {
    // no shrinking
    assert(target > capacity);
    auto newData = new T[target]();
    // non-overlapping memory can be moved
    auto start = begin();
    auto finish = end();
    assert((newData < start && (newData + capacity) < finish) ||
           newData > finish);
    std::move(start, finish, newData);
    if (data != nullptr)
      delete[] data;
    data = newData;
    capacity = target;
  }

  void grow() { resize((std::size_t)((capacity << 1) + 1)); }

 public:
  using comparator_parameter_type = T const&;

  explicit heap(comparator_func<T> comparator) noexcept
      : comparator(comparator) {
    assert(comparator != nullptr);
  }

  heap(const heap& other) : heap(other.comparator) {
    data = new T[other.capacity]();
    std::copy(other.begin(), other.end(), data);
    count_ = other.count();
    capacity = other.capacity;
  }

  heap(heap&& other) : heap(other.comparator) {
    data = other.data;
    other.data = nullptr;
    count_ = other.count();
    capacity = other.capacity;
    comparator = other.comparator;
  }

  heap& operator=(heap const& other) {
    if (&other != this) {
      comparator = other.comparator;
      count_ = other.count_;
      capacity = other.capacity;
      if (data != nullptr) {
        delete[] data;
      }
      data = new T[capacity]();
      std::copy(other.begin(), other.end(), data);
    }
    return *this;
  }

  void put(T t) {
    if (count_ == capacity) {
      grow();
    }
    assert(count_ < capacity);
    data[count_++] = std::move(t);
    heapify();
  }

  T const& peek() const noexcept {
    assert(count_ > 0);
    return data[0];
  }

  T& peek() noexcept {
    assert(count_ > 0);
    return data[0];
  }

  T pop() {
    assert(count_ > 0);

    aswap(data, 0, count_ - 1);
    T value = std::move(data[count_ - 1]);
    count_--;
    sift_down(0);
    return value;
  }

  void reserve(std::size_t capacity) {
    if (capacity > heap::capacity)
      resize(capacity);
  }

#if __has_builtin
#if __has_builtin(__builtin_clzl)
#define CAN_CLZL 1
#endif
#endif

#if CAN_CLZL

  std::size_t depth() const noexcept {
    return sizeof(count()) - __builtin_clzl(count());
  }

#else

  std::size_t depth() const noexcept {
    std::size_t highest = 0;
    auto c = count();
    int pos = 1;
    while (c) {
      if (c & 1)
        highest = pos;

      c >>= 1;
      pos++;
    }
    return highest;
  }

#endif

  auto* begin() noexcept { return data; }

  auto const* begin() const noexcept { return data; }

  auto* end() noexcept { return data + count_; }

  auto const* end() const noexcept { return data + count_; }

  [[nodiscard]] std::size_t count() const noexcept { return count_; }

  [[nodiscard]] int is_empty() const noexcept { return count() == 0; }

  static heap max_heap() {
    return heap{
        +[](T const& a, T const& b) { return a > b ? 1 : (a == b ? 0 : -1); }};
  }

  static heap min_heap() {
    return heap{
        +[](T const& a, T const& b) { return a > b ? -1 : (a == b ? 0 : 1); }};
  }

  ~heap() noexcept {
    if (data != nullptr)
      delete[] data;
  }
};

}  // namespace tom
#endif  // MAXHEAP_HEAP_H
