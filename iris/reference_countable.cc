#include "iris/reference_countable.h"

#include <limits>

#include "absl/log/check.h"

namespace iris {

void ReferenceCountable::Increment() noexcept {
  auto old_value = reference_count_.fetch_add(1u);
  CHECK(old_value != std::numeric_limits<reference_count_t>::max());
}

bool ReferenceCountable::Decrement() noexcept {
  auto old_value = reference_count_.fetch_sub(1u);
  CHECK(old_value != 0);
  return old_value == 1;
}

}  // namespace iris