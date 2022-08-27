#ifndef _IRIS_SCENE_
#define _IRIS_SCENE_

#include <functional>
#include <utility>

#include "iris/intersector.h"
#include "iris/ray.h"

namespace iris {

class Scene {
 public:
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