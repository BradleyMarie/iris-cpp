#include "iris/scene_objects.h"

#include <limits>

#include "absl/log/check.h"

namespace iris {

void SceneObjects::Builder::Add(ReferenceCounted<Geometry> geometry,
                                const Matrix& matrix) {
  if (!geometry) {
    return;
  }

  auto insertion_result = matrix_to_transform_index_.insert(
      {matrix, matrix_to_transform_index_.size()});
  if (insertion_result.second) {
    matrices_.push_back(matrix);
  }

  CHECK(insertion_result.first->second <=
        std::numeric_limits<uintptr_t>::max());
  uintptr_t matrix_index =
      static_cast<uintptr_t>(insertion_result.first->second);

  geometry_.push_back(
      std::make_pair(geometry, reinterpret_cast<const Matrix*>(matrix_index)));
}

void SceneObjects::Builder::Add(ReferenceCounted<Light> light) {
  if (!light) {
    return;
  }

  lights_.push_back(std::move(light));
}

SceneObjects SceneObjects::Builder::Build() {
  assert((std::set<std::pair<ReferenceCounted<Geometry>, const Matrix*>>(
              geometry_.begin(), geometry_.end())
              .size() == geometry_.size()));

  SceneObjects result(std::move(*this));

  matrix_to_transform_index_.clear();
  matrices_.clear();
  geometry_.clear();
  lights_.clear();

  matrix_to_transform_index_.insert({Matrix::Identity(), 0});
  matrices_.push_back(Matrix::Identity());

  return result;
}

SceneObjects::SceneObjects(Builder&& builder)
    : geometry_(std::move(builder.geometry_)),
      lights_(std::move(builder.lights_)),
      matrices_(std::move(builder.matrices_)) {
  matrices_.shrink_to_fit();
  geometry_.shrink_to_fit();
  lights_.shrink_to_fit();

  for (auto& entry : geometry_) {
    if (entry.second) {
      entry.second = &matrices_.at(reinterpret_cast<uintptr_t>(entry.second));
    }
  }
}

}  // namespace iris