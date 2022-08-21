#ifndef _IRIS_INTERNAL_ARENA_
#define _IRIS_INTERNAL_ARENA_

#include <cstddef>
#include <vector>

namespace iris::internal {

class Arena {
 public:
  void* Allocate(size_t size);
  void Clear();

 private:
  std::vector<std::vector<char>> data_;
  size_t data_index_ = 0;
};

}  // namespace iris::internal

#endif  // _IRIS_INTERNAL_ARENA_