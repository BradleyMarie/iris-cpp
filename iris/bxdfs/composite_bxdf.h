#ifndef _IRIS_BXDFS_COMPOSITE_BXDF_
#define _IRIS_BXDFS_COMPOSITE_BXDF_

#include <array>
#include <cassert>

#include "iris/bxdf.h"
#include "iris/float.h"
#include "iris/reflector.h"
#include "iris/sampler.h"
#include "iris/spectral_allocator.h"
#include "iris/vector.h"

namespace iris {
namespace bxdfs {

template <typename... Bxdfs>
class CompositeBxdf final : public Bxdf {
 public:
  CompositeBxdf(Bxdfs&&... bxdfs) {
    size_t i = 0;
    (void(bxdfs_[i++] = &bxdfs), ...);
  }

  Vector Sample(const Vector& incoming, Sampler& sampler) const override {
    size_t index = sampler.NextIndex(sizeof...(Bxdfs));
    return bxdfs_[index]->Sample(incoming, sampler);
  }

  std::optional<visual_t> Pdf(const Vector& incoming, const Vector& outgoing,
                              SampleSource sample_source) const override {
    visual_t diffuse_pdf = 0.0;
    size_t num_diffuse = 0;
    for (const auto* bxdf : bxdfs_) {
      auto pdf = bxdf->Pdf(incoming, outgoing, sample_source);
      diffuse_pdf += pdf.value_or(0.0);
      num_diffuse += pdf.has_value();
    }

    std::optional<visual_t> result;
    if (num_diffuse != 0) {
      visual_t specular_pdf =
          (sample_source == Bxdf::SampleSource::LIGHT) ? 0.0 : 1.0;
      result =
          (diffuse_pdf + static_cast<visual_t>(sizeof...(Bxdfs) - num_diffuse) *
                             specular_pdf) /
          static_cast<visual_t>(sizeof...(Bxdfs));
    }

    return result;
  }

  const Reflector* Reflectance(const Vector& incoming, const Vector& outgoing,
                               SampleSource sample_source,
                               Hemisphere hemisphere,
                               SpectralAllocator& allocator) const override {
    const Reflector* result = nullptr;
    for (const auto* bxdf : bxdfs_) {
      auto reflectance = bxdf->Reflectance(incoming, outgoing, sample_source,
                                           hemisphere, allocator);
      result = allocator.Add(result, reflectance);
    }
    return result;
  }

 private:
  std::array<const Bxdf*, sizeof...(Bxdfs)> bxdfs_;
  static_assert(sizeof...(Bxdfs) != 0);
};

template <typename... Bxdfs>
CompositeBxdf<Bxdfs...> MakeComposite(Bxdfs&&... bxdfs) {
  return CompositeBxdf<Bxdfs...>(bxdfs...);
}

}  // namespace bxdfs
}  // namespace iris

#endif  // _IRIS_BXDFS_COMPOSITE_BXDF_