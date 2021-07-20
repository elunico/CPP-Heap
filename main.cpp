#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>
#include "heap.h"

template <typename T>
void dump_heap(tom::heap<T> const& heap) {
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

static int vec_compare(Vector3D const& self, Vector3D const& that) {
  double a = self.mag();
  double b = self.mag();
  return a == b ? 0 : (a > b ? 1 : -1);
}

void original_test() {
  auto heap = tom::heap<Vector3D>{vec_compare};

  auto other = tom::heap<std::unique_ptr<Vector3D>>{
      [](std::unique_ptr<Vector3D> const& self,
         std::unique_ptr<Vector3D> const& other) {
        return vec_compare(*self, *other);
      }};

  std::cout << "Size of a tom::heap " << sizeof(heap) << std::endl;
  std::cout << "Size of a Vector3D " << sizeof(Vector3D{}) << std::endl;

  int count = ((int)((rand() / (double)RAND_MAX) * 55500)) + 550;
  std::cout << "Count of loops " << count << std::endl;

  for (int i = 0; i < count; i++) {
    heap.put(Vector3D::random());
  }
  assert(heap.count() == count);

  for (int i = 0; i < count; i++) {
    other.put(std::make_unique<Vector3D>(Vector3D::random()));
  }
  assert(other.count() == count);

  std::cout << "Depth of value heap " << heap.depth()
            << "; Count of values: " << heap.count() << std::endl;

  // TODO: ./cpp-heap 1092 fails this assertion because depth is 11 but count is
  // 1024
  SHOW_EXPR(heap.depth());
  SHOW_EXPR(log2(heap.count()));
  SHOW_EXPR(ceil(log2(heap.count())));
  assert(ceil(log2(heap.count())) == heap.depth());

  {
    tom::heap<Vector3D> inner{vec_compare};
    inner.put(Vector3D::random());
    inner = heap;
    std::cout << inner.pop().mag() << std::endl;
    assert(inner.count() == count - 1);
  }
  assert(heap.count() == count);

  std::cout << "Done copy heap" << std::endl;

  {
    auto next = std::move(other);
    std::cout << next.pop()->mag() << std::endl;
  }

  std::cout << "Done move heap" << std::endl;

  for (int i = 0; i < 500; i++) {
    auto p = heap.pop();

    std::cout << p.mag() << std::endl;
  }

  std::cout << std::endl;
}

template <typename T>
void heap_sort_in_place(std::vector<T>& vec) {
  auto heap = tom::heap<T>::min_heap();
  for (auto& a : vec)
    heap.put(std::move(a));

  for (int i = 0; i < vec.size(); i++)
    vec[i] = heap.pop();
}

int random_int(int lower, int higher) {
  return (int)(((rand() / (double)RAND_MAX) * (higher - lower)) + lower);
}

struct T {
  double x, y;
};

int main(int argc, char const* argv[]) {
  static_assert(std::is_move_constructible<Vector3D>::value,
                "Vector3D is not move constructible");
  static_assert(std::is_move_assignable<Vector3D>::value,
                "Vector3D is not move assignable");
  if (argc == 2) {
    std::srand(std::atol(argv[1]));
  } else {
    std::srand(0UL);
  }

  tom::heap<T> z{[](auto a, auto b) { return 1; }};
  for (int i = 0; i < 20; i++) {
    z.put(T{1, 2});
  }

  original_test();

  std::vector<int> v{};
  int const max = 5000;
  v.reserve(max);
  for (int i = 0; i < max; i++) {
    v.push_back(random_int(0, max));
  }

  heap_sort_in_place(v);

  for (auto const& a : v) {
    std::cout << a << "\n";
  }
  std::cout << std::endl;

  return 0;
}
