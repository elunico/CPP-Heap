#include <array>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include "heap.h"

void dump_heap(tom::heap<int> const& heap) {
  for (auto const& a : heap)
    std::cout << a << " ";
  std::cout << std::endl;
}

struct Vector3D {
  double x, y, z;

  [[nodiscard]] double mag() const {
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
  }

  static Vector3D random();
};

Vector3D Vector3D::random() {
  return Vector3D{rand() / (double)RAND_MAX, rand() / (double)RAND_MAX,
                  rand() / (double)RAND_MAX};
}

int main() {
  auto heap =
      tom::heap<Vector3D>{[](Vector3D const& self, Vector3D const& other) {
        double mag_a = self.mag();
        double mag_b = other.mag();
        return mag_a == mag_b ? 0 : (mag_a > mag_b ? 1 : -1);
      }};

  auto other = tom::heap<std::unique_ptr<Vector3D>>{
      [](std::unique_ptr<Vector3D> const& self,
         std::unique_ptr<Vector3D> const& other) {
        double mag_a = self->mag();
        double mag_b = other->mag();
        return mag_a == mag_b ? 0 : (mag_a > mag_b ? 1 : -1);
      }};

  std::cout << "Size of a tom::heap " << sizeof(heap) << std::endl;
  std::cout << "Size of a Vector3D " << sizeof(Vector3D{}) << std::endl;

  {
    auto inner = heap;
    std::cout << inner.pop().mag() << std::endl;
  }

  for (int i = 0; i < 1000; i++) {
    heap.put(Vector3D::random());
  }

  for (int i = 0; i < 1000; i++) {
    other.put(std::make_unique<Vector3D>(Vector3D::random()));
  }

  for (int i = 0; i < 500; i++) {
    auto p = heap.pop();
    auto q = heap.pop();

    std::cout << p.mag() << std::endl;
    std::cout << q.mag() << std::endl;
  }

  std::cout << std::endl;

  return 0;
}
