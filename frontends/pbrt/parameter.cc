#include "frontends/pbrt/parameter.h"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

namespace iris::pbrt_frontend {
namespace {

template <ParameterList::Type type, typename StoredType>
void ParseSimpleType(
    const ParameterList& parameter_list,
    const std::vector<StoredType>& (ParameterList::*get_source)() const,
    std::vector<StoredType>& destination) {
  if (parameter_list.GetType() != type) {
    std::cerr << "ERROR: Wrong type for parameter list: "
              << parameter_list.GetName() << std::endl;
    exit(EXIT_FAILURE);
  }

  destination.clear();
  for (const auto& entry : (parameter_list.*get_source)()) {
    destination.emplace_back(entry);
  }
}

template <Parameter::Type type, typename ReturnType>
const std::vector<ReturnType>& GetValues(const Parameter& parameter,
                                         const std::vector<ReturnType>& result,
                                         size_t max_num_values,
                                         size_t min_num_values) {
  assert(type == parameter.GetType());
  if (result.size() < min_num_values) {
    std::cerr << "ERROR: Too few values in parameter list: "
              << parameter.GetName() << std::endl;
    exit(EXIT_FAILURE);
  }

  if (max_num_values != 0 && result.size() > max_num_values) {
    std::cerr << "ERROR: Too many values in parameter list: "
              << parameter.GetName() << std::endl;
    exit(EXIT_FAILURE);
  }

  return result;
}

}  // namespace

std::string_view Parameter::GetName() const { return name_.value(); }

Parameter::Type Parameter::GetType() const { return type_.value(); }

const std::vector<bool>& Parameter::GetBoolValues(size_t max_num_values,
                                                  size_t min_num_values) const {
  return GetValues<BOOL>(*this, bools_, max_num_values, min_num_values);
}

const std::vector<long double>& Parameter::GetFloatValues(
    size_t max_num_values, size_t min_num_values) const {
  return GetValues<FLOAT>(*this, floats_, max_num_values, min_num_values);
}

const std::vector<ReferenceCounted<textures::ValueTexture2D<visual>>>&
Parameter::GetFloatTextures(size_t max_num_values,
                            size_t min_num_values) const {
  return GetValues<FLOAT_TEXTURE>(*this, float_textures_, max_num_values,
                                  min_num_values);
}

const std::vector<int64_t>& Parameter::GetIntegerValues(
    size_t max_num_values, size_t min_num_values) const {
  return GetValues<INTEGER>(*this, integers_, max_num_values, min_num_values);
}

const std::vector<Vector>& Parameter::GetNormalValues(
    size_t max_num_values, size_t min_num_values) const {
  return GetValues<NORMAL>(*this, vectors_, max_num_values, min_num_values);
}

const std::vector<Point>& Parameter::GetPoint3Values(
    size_t max_num_values, size_t min_num_values) const {
  return GetValues<POINT3>(*this, points_, max_num_values, min_num_values);
}

const std::vector<
    ReferenceCounted<textures::PointerTexture2D<Reflector, SpectralAllocator>>>&
Parameter::GetReflectorTextures(size_t max_num_values,
                                size_t min_num_values) const {
  return GetValues<REFLECTOR_TEXTURE>(*this, reflector_textures_,
                                      max_num_values, min_num_values);
}

const std::vector<std::string_view>& Parameter::GetStringValues(
    size_t max_num_values, size_t min_num_values) const {
  return GetValues<STRING>(*this, strings_, max_num_values, min_num_values);
}

const std::vector<iris::ReferenceCounted<Spectrum>>& Parameter::GetSpectra(
    size_t max_num_values, size_t min_num_values) const {
  return GetValues<SPECTRUM>(*this, spectra_, max_num_values, min_num_values);
}

const std::vector<Vector>& Parameter::GetVector3Values(
    size_t max_num_values, size_t min_num_values) const {
  return GetValues<VECTOR3>(*this, vectors_, max_num_values, min_num_values);
}

void Parameter::ParseBool(const ParameterList& parameter_list,
                          SpectrumManager& spectrum_manager,
                          TextureManager& texture_manager) {
  ParseSimpleType<ParameterList::BOOL>(parameter_list,
                                       &ParameterList::GetBoolValues, bools_);
}

void Parameter::ParseFloat(const ParameterList& parameter_list,
                           SpectrumManager& spectrum_manager,
                           TextureManager& texture_manager) {
  ParseSimpleType<ParameterList::FLOAT>(
      parameter_list, &ParameterList::GetFloatValues, floats_);
}

void Parameter::ParseFloatTexture(const ParameterList& parameter_list,
                                  SpectrumManager& spectrum_manager,
                                  TextureManager& texture_manager) {
  float_textures_.clear();
  if (parameter_list.GetType() == ParameterList::FLOAT) {
    for (const auto& entry : parameter_list.GetFloatValues()) {
      visual as_visual = static_cast<visual>(entry);
      if (!std::isfinite(as_visual)) {
        std::cerr << "ERROR: Out of range value in parameter list: "
                  << GetName() << std::endl;
        exit(EXIT_FAILURE);
      }

      float_textures_.push_back(
          texture_manager.AllocateUniformFloatTexture(as_visual));
    }
    return;
  }

  if (parameter_list.GetType() == ParameterList::TEXTURE) {
    for (const auto& entry : parameter_list.GetTextureValues()) {
      float_textures_.push_back(texture_manager.GetFloatTexture(entry));
    }
    return;
  }

  std::cerr << "ERROR: Wrong type for parameter list: "
            << parameter_list.GetName() << std::endl;
  exit(EXIT_FAILURE);
}

void Parameter::ParseInteger(const ParameterList& parameter_list,
                             SpectrumManager& spectrum_manager,
                             TextureManager& texture_manager) {
  ParseSimpleType<ParameterList::INTEGER>(
      parameter_list, &ParameterList::GetIntegerValues, integers_);
}

void Parameter::ParseNormal(const ParameterList& parameter_list,
                            SpectrumManager& spectrum_manager,
                            TextureManager& texture_manager) {
  ParseSimpleType<ParameterList::NORMAL>(
      parameter_list, &ParameterList::GetNormalValues, vectors_);
}

void Parameter::ParsePoint3(const ParameterList& parameter_list,
                            SpectrumManager& spectrum_manager,
                            TextureManager& texture_manager) {
  ParseSimpleType<ParameterList::POINT3>(
      parameter_list, &ParameterList::GetPoint3Values, points_);
}

void Parameter::ParseReflectorTexture(const ParameterList& parameter_list,
                                      SpectrumManager& spectrum_manager,
                                      TextureManager& texture_manager) {
  reflector_textures_.clear();
  if (parameter_list.GetType() == ParameterList::COLOR) {
    for (const auto& entry : parameter_list.GetColorValues()) {
      if (entry.values[0] > 1.0 || entry.values[1] > 1.0 ||
          entry.values[2] > 1.0) {
        std::cerr << "ERROR: Out of range value in parameter list: "
                  << GetName() << std::endl;
        exit(EXIT_FAILURE);
      }

      reflector_textures_.push_back(
          texture_manager.AllocateUniformReflectorTexture(
              spectrum_manager.AllocateReflector(entry)));
    }
    return;
  }

  if (parameter_list.GetType() == ParameterList::SPECTRUM) {
    for (const auto& entry : parameter_list.GetSpectrumValues()) {
      if (entry.second > 1.0) {
        std::cerr << "ERROR: Out of range value in parameter list: "
                  << GetName() << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    reflector_textures_.push_back(
        texture_manager.AllocateUniformReflectorTexture(
            spectrum_manager.AllocateReflector(
                parameter_list.GetSpectrumValues())));
    return;
  }

  if (parameter_list.GetType() == ParameterList::TEXTURE) {
    for (const auto& entry : parameter_list.GetTextureValues()) {
      reflector_textures_.push_back(texture_manager.GetReflectorTexture(entry));
    }
    return;
  }

  std::cerr << "ERROR: Wrong type for parameter list: "
            << parameter_list.GetName() << std::endl;
  exit(EXIT_FAILURE);
}

void Parameter::ParseSpectrum(const ParameterList& parameter_list,
                              SpectrumManager& spectrum_manager,
                              TextureManager& texture_manager) {
  spectra_.clear();
  if (parameter_list.GetType() == ParameterList::COLOR) {
    for (const auto& entry : parameter_list.GetColorValues()) {
      spectra_.push_back(spectrum_manager.AllocateSpectrum(entry));
    }
    return;
  }

  if (parameter_list.GetType() == ParameterList::SPECTRUM) {
    spectra_.push_back(
        spectrum_manager.AllocateSpectrum(parameter_list.GetSpectrumValues()));
    return;
  }

  std::cerr << "ERROR: Wrong type for parameter list: "
            << parameter_list.GetName() << std::endl;
  exit(EXIT_FAILURE);
}

void Parameter::ParseString(const ParameterList& parameter_list,
                            SpectrumManager& spectrum_manager,
                            TextureManager& texture_manager) {
  if (parameter_list.GetType() != ParameterList::STRING) {
    std::cerr << "ERROR: Wrong type for parameter list: "
              << parameter_list.GetName() << std::endl;
    exit(EXIT_FAILURE);
  }

  const auto& string_list = parameter_list.GetStringValues();
  for (size_t i = 0; i < string_list.size(); i++) {
    if (i == string_storage_.size()) {
      string_storage_.emplace_back(string_list[i]);
    } else {
      string_storage_[i] = string_list[i];
    }
  }

  strings_.clear();
  for (size_t i = 0; i < string_list.size(); i++) {
    strings_.emplace_back(string_storage_[i]);
  }
}

void Parameter::ParseVector3(const ParameterList& parameter_list,
                             SpectrumManager& spectrum_manager,
                             TextureManager& texture_manager) {
  ParseSimpleType<ParameterList::VECTOR3>(
      parameter_list, &ParameterList::GetVector3Values, vectors_);
}

void Parameter::LoadFrom(const ParameterList& parameter_list, Type type,
                         SpectrumManager& spectrum_manager,
                         TextureManager& texture_manager) {
  static std::unordered_map<Type, void (Parameter::*)(const ParameterList&,
                                                      SpectrumManager&,
                                                      TextureManager&)>
      callbacks = {
          {BOOL, &Parameter::ParseBool},
          {FLOAT, &Parameter::ParseFloat},
          {FLOAT_TEXTURE, &Parameter::ParseFloatTexture},
          {INTEGER, &Parameter::ParseInteger},
          {NORMAL, &Parameter::ParseNormal},
          {POINT3, &Parameter::ParsePoint3},
          {REFLECTOR_TEXTURE, &Parameter::ParseReflectorTexture},
          {SPECTRUM, &Parameter::ParseSpectrum},
          {STRING, &Parameter::ParseString},
          {VECTOR3, &Parameter::ParseVector3},
      };

  name_storage_ = parameter_list.GetName();
  name_ = name_storage_;
  type_ = type;

  auto callback = callbacks.at(type);
  (this->*callback)(parameter_list, spectrum_manager, texture_manager);
}

}  // namespace iris::pbrt_frontend