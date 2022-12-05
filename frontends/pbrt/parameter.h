#ifndef _FRONTENDS_PBRT_PARAMETER_
#define _FRONTENDS_PBRT_PARAMETER_

#include <span>
#include <string_view>

#include "frontends/pbrt/color.h"
#include "frontends/pbrt/parameter_list.h"
#include "frontends/pbrt/spectrum_manager.h"
#include "frontends/pbrt/texture_manager.h"
#include "iris/reference_counted.h"
#include "iris/reflector.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/textures/texture2d.h"

namespace iris::pbrt_frontend {

class Parameter {
 public:
  enum Type {
    BOOL,
    FLOAT,
    FLOAT_TEXTURE,
    INTEGER,
    NORMAL,
    POINT3,
    REFLECTOR_TEXTURE,
    SPECTRUM,
    STRING,
    VECTOR3,
  };

  std::string_view GetName() const;
  Type GetType() const;

  const std::vector<bool>& GetBoolValues(size_t max_num_values = 0,
                                         size_t min_num_values = 1) const;

  const std::vector<long double>& GetFloatValues(
      size_t max_num_values = 0, size_t min_num_values = 1) const;

  const std::vector<ReferenceCounted<textures::ValueTexture2D<visual>>>&
  GetFloatTextures(size_t max_num_values = 0, size_t min_num_values = 1) const;

  const std::vector<int64_t>& GetIntegerValues(size_t max_num_values = 0,
                                               size_t min_num_values = 1) const;

  const std::vector<Vector>& GetNormalValues(size_t max_num_values = 0,
                                             size_t min_num_values = 1) const;

  const std::vector<Point>& GetPoint3Values(size_t max_num_values = 0,
                                            size_t min_num_values = 1) const;

  const std::vector<ReferenceCounted<
      textures::PointerTexture2D<Reflector, SpectralAllocator>>>&
  GetReflectorTextures(size_t max_num_values = 0,
                       size_t min_num_values = 1) const;

  const std::vector<std::string_view>& GetStringValues(
      size_t max_num_values = 0, size_t min_num_values = 1) const;

  const std::vector<iris::ReferenceCounted<Spectrum>>& GetSpectra(
      size_t max_num_values = 0, size_t min_num_values = 1) const;

  const std::vector<Vector>& GetVector3Values(size_t max_num_values = 0,
                                              size_t min_num_values = 1) const;

  void LoadFrom(const ParameterList& parameter_list, Type type,
                SpectrumManager& spectrum_manager,
                TextureManager& texture_manager);

 private:
  void ParseBool(const ParameterList& parameter_list,
                 SpectrumManager& spectrum_manager,
                 TextureManager& texture_manager);
  void ParseFloat(const ParameterList& parameter_list,
                  SpectrumManager& spectrum_manager,
                  TextureManager& texture_manager);
  void ParseFloatTexture(const ParameterList& parameter_list,
                         SpectrumManager& spectrum_manager,
                         TextureManager& texture_manager);
  void ParseInteger(const ParameterList& parameter_list,
                    SpectrumManager& spectrum_manager,
                    TextureManager& texture_manager);
  void ParseNormal(const ParameterList& parameter_list,
                   SpectrumManager& spectrum_manager,
                   TextureManager& texture_manager);
  void ParsePoint3(const ParameterList& parameter_list,
                   SpectrumManager& spectrum_manager,
                   TextureManager& texture_manager);
  void ParseReflectorTexture(const ParameterList& parameter_list,
                             SpectrumManager& spectrum_manager,
                             TextureManager& texture_manager);
  void ParseSpectrum(const ParameterList& parameter_list,
                     SpectrumManager& spectrum_manager,
                     TextureManager& texture_manager);
  void ParseString(const ParameterList& parameter_list,
                   SpectrumManager& spectrum_manager,
                   TextureManager& texture_manager);
  void ParseVector3(const ParameterList& parameter_list,
                    SpectrumManager& spectrum_manager,
                    TextureManager& texture_manager);

  std::optional<std::string> name_;
  std::optional<Type> type_;

  std::vector<bool> bools_;
  std::vector<Color> colors_;
  std::vector<long double> floats_;
  std::vector<ReferenceCounted<textures::ValueTexture2D<visual>>>
      float_textures_;
  std::vector<int64_t> integers_;
  std::vector<ReferenceCounted<
      textures::PointerTexture2D<Reflector, SpectralAllocator>>>
      reflector_textures_;
  std::vector<iris::ReferenceCounted<Spectrum>> spectra_;
  std::vector<std::string_view> strings_;
  std::vector<std::string> string_storage_;
  std::vector<Point> points_;
  std::vector<Vector> vectors_;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_PARAMETER_