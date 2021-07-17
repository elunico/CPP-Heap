#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include "heap.h"

void dump_heap(tom::heap<int> const& heap) {
  for (auto const& a : heap)
    std::cout << a << " ";
  std::cout << std::endl;
}

struct Vector3D {
  double x, y, z;

  double mag() const { return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }

  static Vector3D random();
};

Vector3D Vector3D::random() {
  return Vector3D{rand() / (double)RAND_MAX, rand() / (double)RAND_MAX,
                  rand() / (double)RAND_MAX};
}

int main() {
  auto heap = tom::heap<Vector3D>{[](Vector3D self, Vector3D other) {
    double mag_a = self.mag();
    double mag_b = other.mag();
    return mag_a == mag_b ? 0 : (mag_a > mag_b ? 1 : -1);
  }};

  std::cout << "Size of a tom::heap " << sizeof(heap) << std::endl;
  std::cout << "Size of a Vector3D " << sizeof(Vector3D{}) << std::endl;

  for (int i = 0; i < 100000; i++)
    heap.put(Vector3D::random());

  while (heap.count() > 0) {
    auto p = heap.pop();
    if (p.mag() > 1.45) {
      std::cout << p.mag() << std::endl;
    }
  }

  std::cout << std::endl;

  return 0;
}
