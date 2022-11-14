#include "frontends/pbrt/parameter.h"

#include <cstdlib>
#include <iostream>
#include <unordered_map>

namespace iris::pbrt_frontend {
namespace {

template <ParameterList::Type type, typename StoredType>
void ParseSimpleType(const ParameterList& parameter_list,
                     const std::vector<StoredType>& source,
                     std::vector<StoredType>& destination) {
  if (parameter_list.GetType() != type) {
    std::cerr << "ERROR: Wrong type for parameter list: "
              << parameter_list.GetName() << std::endl;
    exit(EXIT_FAILURE);
  }

  destination.clear();
  for (const auto& entry : source) {
    destination.emplace_back(entry);
  }
}

}  // namespace

std::string_view Parameter::GetName() const { return name_.value(); }

Parameter::Type Parameter::GetType() const { return type_.value(); }

void Parameter::ParseBool(const ParameterList& parameter_list,
                          SpectrumManager& spectrum_manager,
                          TextureManager& texture_manager) {
  ParseSimpleType<ParameterList::BOOL>(parameter_list,
                                       parameter_list.GetBoolValues(), bools_);
}

void Parameter::ParseFloat(const ParameterList& parameter_list,
                           SpectrumManager& spectrum_manager,
                           TextureManager& texture_manager) {
  ParseSimpleType<ParameterList::FLOAT>(
      parameter_list, parameter_list.GetFloatValues(), floats_);
}

void Parameter::ParseFloatTexture(const ParameterList& parameter_list,
                                  SpectrumManager& spectrum_manager,
                                  TextureManager& texture_manager) {
  float_textures_.clear();
  if (parameter_list.GetType() == ParameterList::FLOAT) {
    for (const auto& entry : parameter_list.GetFloatValues()) {
      float_textures_.push_back(
          texture_manager.AllocateUniformFloatTexture(entry));
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
      parameter_list, parameter_list.GetIntegerValues(), integers_);
}

void Parameter::ParseNormal(const ParameterList& parameter_list,
                            SpectrumManager& spectrum_manager,
                            TextureManager& texture_manager) {
  ParseSimpleType<ParameterList::NORMAL>(
      parameter_list, parameter_list.GetNormalValues(), vectors_);
}

void Parameter::ParsePoint3(const ParameterList& parameter_list,
                            SpectrumManager& spectrum_manager,
                            TextureManager& texture_manager) {
  ParseSimpleType<ParameterList::POINT3>(
      parameter_list, parameter_list.GetPoint3Values(), points_);
}

void Parameter::ParseReflectorTexture(const ParameterList& parameter_list,
                                      SpectrumManager& spectrum_manager,
                                      TextureManager& texture_manager) {
  reflector_textures_.clear();
  if (parameter_list.GetType() == ParameterList::FLOAT) {
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
      parameter_list, parameter_list.GetVector3Values(), vectors_);
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