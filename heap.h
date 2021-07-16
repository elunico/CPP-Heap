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
  T const val = arr[a];
  arr[a] = arr[b];
  arr[b] = val;
}

template <typename T>
using comparator_func = int (*)(T, T);

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
    auto newData = new T[target]();
    std::copy(begin(), end(), newData);
    if (data != nullptr)
      delete[] data;
    data = newData;
    capacity = target;
  }

  void grow() { resize((std::size_t)((capacity << 1) + 1)); }

 public:
  explicit heap(int (*comparator)(T, T)) noexcept : comparator(comparator) {}

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

  heap& operator=(heap const& other) { return *this; }

  void put(T t) {
    if (count_ == capacity) {
      grow();
    }
    assert(count_ < capacity);
    data[count_++] = t;
    heapify();
  }

  T const& peek() const noexcept {
    assert(capacity > 0);
    return data[0];
  }

  T& peek() noexcept {
    assert(capacity > 0);
    return data[0];
  }

  T pop() {
    assert(count_ - 1 >= 0);

    aswap(data, 0, count_ - 1);
    T value = data[count_ - 1];
    count_--;
    sift_down(0);
    return value;
  }

  void reserve(std::size_t capacity) {
    if (capacity > heap::capacity)
      resize(capacity);
  }

  auto* begin() noexcept { return data; }

  auto const* begin() const noexcept { return data; }

  auto* end() noexcept { return data + count_; }

  auto const* end() const noexcept { return data + count_; }

  [[nodiscard]] std::size_t count() const noexcept { return count_; }

  static heap max_heap() {
    return heap{+[](T a, T b) { return a > b ? 1 : (a == b ? 0 : -1); }};
  }

  static heap min_heap() {
    return heap{+[](T a, T b) { return a > b ? 1 : (a == b ? 0 : -1); }};
  }

  ~heap() noexcept { delete[] data; }
};

}  // namespace tom
#endif  // MAXHEAP_HEAP_H
