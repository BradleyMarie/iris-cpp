#ifndef _IRIS_SCENE_
#define _IRIS_SCENE_

#include <functional>
#include <map>
#include <set>
#include <utility>

#include "iris/geometry.h"
#include "iris/intersector.h"
#include "iris/matrix.h"
#include "iris/ray.h"

namespace iris {

class Scene {
 public:
  class Builder {
   public:
    void Add(std::unique_ptr<Geometry> geometry,
             const Matrix& matrix = Matrix::Identity());
    void Add(const Geometry* geometry,
             const Matrix& matrix = Matrix::Identity());
    std::unique_ptr<Scene> Build();

   private:
    virtual std::unique_ptr<Scene> Build(
        std::vector<std::pair<size_t, size_t>> geometry_and_matrix,
        std::vector<std::unique_ptr<Geometry>> geometry,
        std::vector<Matrix> matrices) = 0;

    std::set<std::pair<size_t, size_t>> indices_;
    std::vector<std::unique_ptr<Geometry>> geometry_;
    std::map<const Geometry*, size_t> numbered_geometry_;
    std::map<Matrix, size_t> numbered_matrices_ = {{Matrix::Identity(), 0}};
  };

  class const_iterator {
   public:
    typedef std::pair<const Geometry&, const Matrix*> value_type;

    explicit const_iterator(
        std::function<std::optional<value_type>()> generator) noexcept
        : generator_(std::move(generator)) {
      ++(*this);
    }

    const_iterator(const const_iterator& iter) = default;
    const_iterator& operator=(const const_iterator& iter) = default;

    value_type operator*() noexcept;

    const_iterator& operator++() noexcept;
    const_iterator operator++(int);

    bool operator==(const const_iterator& iter) const noexcept;
    bool operator!=(const const_iterator& iter) const noexcept;

   private:
    std::function<std::optional<value_type>()> generator_;
    const Geometry* geometry_ = nullptr;
    const Matrix* matrix_ = nullptr;
  };

  virtual const_iterator begin() const = 0;
  const_iterator end() const;

  virtual void Trace(const Ray& ray, Intersector& intersector) const = 0;
  virtual ~Scene() {}
};

}  // namespace iris

#endif  // _IRIS_SCENE_