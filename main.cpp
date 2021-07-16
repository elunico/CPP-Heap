#include "heap.h"
#include <array>
#include <cmath>
#include <iostream>
#include <vector>

void dump_heap(tom::heap<int> const &heap) {
  for (auto const &a : heap)
    std::cout << a << " ";
  std::cout << std::endl;
}

struct Vector3D {
  double x, y, z;

  int operator>(Vector3D const &other) {
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)) >
                   sqrt(pow(other.x, 2) + pow(other.y, 2) + pow(other.z, 2))
               ? 1
               : -1;
  }

  int operator==(Vector3D const &other) {
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)) ==
           sqrt(pow(other.x, 2) + pow(other.y, 2) + pow(other.z, 2));
  }
};

int main() {

  auto heap = tom::heap<Vector3D>::max_heap();

  std::cout << "Size of a tom::heap " << sizeof(heap) << std::endl;
  std::cout << "Size of a Vector3D " << sizeof(Vector3D) << std::endl;

  for (int i = 0; i < 500000; i++)
    heap.put(Vector3D{rand() / (double)RAND_MAX, rand() / (double)RAND_MAX,
                      rand() / (double)RAND_MAX});

  // std::vector<int> v{};
  // for (int i = 0; i < 500000; i++)
  //   v.push_back(i);

  //   size_t count = heap.count();

  //   for (int i = 0; i < count; i++) {
  //     auto a = heap.pop();
  //     if (a % 351 == 0)
  //       std::cout << a << " " << std::endl;
  //   }
  std::cout << std::endl;

  return 0;
}
