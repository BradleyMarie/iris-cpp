#include "frontends/pbrt/parameter_list.h"

#include <cassert>
#include <cctype>
#include <charconv>
#include <iostream>
#include <unordered_map>
#include <utility>

#include "frontends/pbrt/quoted_string.h"

namespace iris::pbrt_frontend {
namespace {

std::optional<std::pair<std::string_view, std::string_view>> ParseTypeAndName(
    Tokenizer& tokenizer) {
  auto first_token = tokenizer.Peek();
  if (!first_token) {
    return std::nullopt;
  }

  auto unquoted = Unquote(*first_token);
  if (!unquoted) {
    return std::nullopt;
  }

  size_t type_end = unquoted->find_first_of(" \t");
  if (type_end == std::string_view::npos) {
    return std::nullopt;
  }

  std::string_view type = unquoted->substr(0, type_end);
  if (type.empty()) {
    return std::nullopt;
  }

  size_t name_start = unquoted->find_first_not_of(" \t", type_end);
  if (name_start == std::string_view::npos) {
    return std::nullopt;
  }

  size_t name_size = unquoted->size() - name_start;
  std::string_view name = unquoted->substr(name_start, name_size);
  assert(!name.empty());

  if (name.find_first_of(" \t") != std::string_view::npos) {
    return std::nullopt;
  }

  return std::make_pair(type, name);
}

template <typename Type, typename ParseType,
          std::optional<ParseType> (*ParseFunc)(std::string_view)>
void ParseSingle(std::string_view token, std::string_view type_name,
                 size_t insert_index, std::vector<Type>& result) {
  auto parsed = ParseFunc(token);
  if (!parsed) {
    std::cerr << "ERROR: Failed to parse " << type_name << " value: " << token
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (insert_index < result.size()) {
    result[insert_index] = *parsed;
  } else {
    result.emplace_back(*parsed);
  }
}

template <typename Type, typename ParseType,
          std::optional<ParseType> (*ParseFunc)(std::string_view)>
size_t ParseLoop(Tokenizer& tokenizer, std::string_view type_name,
                 std::vector<Type>& result) {
  size_t insert_index = 0;
  for (;; insert_index++) {
    auto token = tokenizer.Next();

    if (!token) {
      std::cerr << "ERROR: Unterminated parameter list" << std::endl;
      exit(EXIT_FAILURE);
    }

    if (*token == "]") {
      break;
    }

    ParseSingle<Type, ParseType, ParseFunc>(*token, type_name, insert_index,
                                            result);
  }

  if (insert_index == 0) {
    std::cerr << "ERROR: Empty parameter list" << std::endl;
    exit(EXIT_FAILURE);
  }

  return insert_index;
}

template <typename Type, typename ParseType,
          std::optional<ParseType> (*ParseFunc)(std::string_view)>
size_t ParseData(Tokenizer& tokenizer, std::string_view type_name,
                 std::vector<Type>& result) {
  auto token = *tokenizer.Next();

  if (token != "[") {
    ParseSingle<Type, ParseType, ParseFunc>(token, type_name, 0, result);
    return 1;
  }

  return ParseLoop<Type, ParseType, ParseFunc>(tokenizer, type_name, result);
}

std::optional<bool> ParseBoolToken(std::string_view token) {
  if (token == "\"true\"") {
    return true;
  } else if (token == "\"false\"") {
    return false;
  }
  return std::nullopt;
}

std::optional<long double> ParseFloatToken(std::string_view token) {
  long double value;
  if (std::from_chars(token.begin(), token.end(), value).ec != std::errc{}) {
    return std::nullopt;
  }
  if (!std::isfinite(value)) {
    return std::nullopt;
  }
  return value;
}

std::optional<int64_t> ParseIntegerToken(std::string_view token) {
  int64_t value;
  if (std::from_chars(token.begin(), token.end(), value).ec != std::errc{}) {
    return std::nullopt;
  }
  return value;
}

std::optional<std::string_view> ParseRawStringToken(std::string_view token) {
  return token;
}

std::optional<std::string_view> ParseStringToken(std::string_view token) {
  auto value = Unquote(token);
  if (!value) {
    return std::nullopt;
  }
  return value;
}

}  // namespace

std::string_view ParameterList::GetName() const { return name_.value(); }

ParameterList::Type ParameterList::GetType() const { return type_.value(); }

const std::vector<bool>& ParameterList::GetBoolValues() const {
  assert(type_.value() == BOOL);
  return bools_;
}

const std::vector<Color> ParameterList::GetColorValues() const {
  assert(type_.value() == COLOR);
  return colors_;
}

const std::vector<long double>& ParameterList::GetFloatValues() const {
  assert(type_.value() == FLOAT);
  return floats_;
}

const std::vector<int64_t> ParameterList::GetIntegerValues() const {
  assert(type_.value() == INTEGER);
  return integers_;
}

const std::vector<Vector> ParameterList::GetNormalValues() const {
  assert(type_.value() == NORMAL);
  return vectors_;
}

const std::vector<Point> ParameterList::GetPoint3Values() const {
  assert(type_.value() == POINT3);
  return points_;
}

const std::vector<std::array<visual, 2>>& ParameterList::GetSpectrumValues()
    const {
  assert(type_.value() == SPECTRUM);
  return spectrum_;
}

const std::vector<std::string_view> ParameterList::GetStringValues() const {
  assert(type_.value() == STRING);
  return strings_;
}

const std::vector<std::string_view> ParameterList::GetTextureValues() const {
  assert(type_.value() == TEXTURE);
  return strings_;
}

const std::vector<Vector> ParameterList::GetVector3Values() const {
  assert(type_.value() == VECTOR3);
  return vectors_;
}

ParameterList::Type ParameterList::ParseBool(Tokenizer& tokenizer,
                                             std::string_view type_name,
                                             Color::Space rgb_color_space) {
  bools_.clear();
  ParseData<bool, bool, ParseBoolToken>(tokenizer, type_name, bools_);
  return BOOL;
}

ParameterList::Type ParameterList::ParseFloat(Tokenizer& tokenizer,
                                              std::string_view type_name,
                                              Color::Space rgb_color_space) {
  floats_.clear();
  ParseData<long double, long double, ParseFloatToken>(tokenizer, type_name,
                                                       floats_);
  return FLOAT;
}

ParameterList::Type ParameterList::ParseInteger(Tokenizer& tokenizer,
                                                std::string_view type_name,
                                                Color::Space rgb_color_space) {
  integers_.clear();
  ParseData<int64_t, int64_t, ParseIntegerToken>(tokenizer, type_name,
                                                 integers_);
  return INTEGER;
}

ParameterList::Type ParameterList::ParseNormal(Tokenizer& tokenizer,
                                               std::string_view type_name,
                                               Color::Space rgb_color_space) {
  ParseVector3(tokenizer, type_name, rgb_color_space);
  return NORMAL;
}

ParameterList::Type ParameterList::ParsePoint3(Tokenizer& tokenizer,
                                               std::string_view type_name,
                                               Color::Space rgb_color_space) {
  ParseFloat(tokenizer, type_name, rgb_color_space);

  if (floats_.size() % 3 != 0) {
    std::cerr << "ERROR: The number of values in a " << type_name
              << " parameter list must be evenly divisible by 3" << std::endl;
    exit(EXIT_FAILURE);
  }

  points_.clear();
  for (size_t i = 0; i < floats_.size(); i += 3) {
    geometric x = static_cast<geometric>(floats_[i]);
    geometric y = static_cast<geometric>(floats_[i + 1]);
    geometric z = static_cast<geometric>(floats_[i + 2]);
    if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z)) {
      std::cerr << "ERROR: A " << type_name
                << " parameter list value was out of range" << std::endl;
      exit(EXIT_FAILURE);
    }

    points_.emplace_back(x, y, z);
  }

  return POINT3;
}

ParameterList::Type ParameterList::ParseRgb(Tokenizer& tokenizer,
                                            std::string_view type_name,
                                            Color::Space rgb_color_space) {
  ParseFloat(tokenizer, type_name, rgb_color_space);

  if (floats_.size() % 3 != 0) {
    std::cerr << "ERROR: The number of values in an " << type_name
              << " parameter list must be evenly divisible by 3" << std::endl;
    exit(EXIT_FAILURE);
  }

  colors_.clear();
  for (size_t i = 0; i < floats_.size(); i += 3) {
    if (floats_[i] < 0.0 || floats_[i + 1] < 0.0 || floats_[i + 2] < 0.0) {
      std::cerr << "ERROR: The values in an " << type_name
                << " parameter list cannot be negative" << std::endl;
      exit(EXIT_FAILURE);
    }

    visual_t r = static_cast<visual_t>(floats_[i]);
    visual_t g = static_cast<visual_t>(floats_[i + 1]);
    visual_t b = static_cast<visual_t>(floats_[i + 2]);
    if (!std::isfinite(r) || !std::isfinite(g) || !std::isfinite(b)) {
      std::cerr << "ERROR: A " << type_name
                << " parameter list value was out of range" << std::endl;
      exit(EXIT_FAILURE);
    }

    colors_.emplace_back(r, g, b, rgb_color_space);
  }

  return COLOR;
}

ParameterList::Type ParameterList::ParseSpectrum(Tokenizer& tokenizer,
                                                 std::string_view type_name,
                                                 Color::Space rgb_color_space) {
  size_t num_added =
      ParseData<std::string, std::string_view, ParseRawStringToken>(
          tokenizer, type_name, string_storage_);

  floats_.clear();
  if (string_storage_[0].empty() || string_storage_[0][0] != '"') {
    for (size_t i = 0; i < num_added; i++) {
      ParseSingle<long double, long double, ParseFloatToken>(
          string_storage_[i], type_name, i, floats_);
    }
  } else {
    // TODO: Parse SPD file
    std::cerr << "ERROR: SPD File Support not implemented" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (floats_.size() % 2 != 0) {
    std::cerr << "ERROR: The number of values in a " << type_name
              << " parameter list cannot be odd" << std::endl;
    exit(EXIT_FAILURE);
  }

  spectrum_.clear();
  for (size_t i = 0; i < floats_.size(); i += 2) {
    if (floats_[i] < 0.0 || floats_[i + 1] < 0.0) {
      std::cerr << "ERROR: The values in an " << type_name
                << " parameter list cannot be negative" << std::endl;
      exit(EXIT_FAILURE);
    }

    visual wavelength = static_cast<visual>(floats_[i]);
    visual intensity = static_cast<visual>(floats_[i + 1]);
    if (!std::isfinite(wavelength) || !std::isfinite(intensity)) {
      std::cerr << "ERROR: A " << type_name
                << " parameter list value was out of range" << std::endl;
      exit(EXIT_FAILURE);
    }

    spectrum_.push_back(std::array<visual, 2>{wavelength, intensity});
  }

  return SPECTRUM;
}

ParameterList::Type ParameterList::ParseString(Tokenizer& tokenizer,
                                               std::string_view type_name,
                                               Color::Space rgb_color_space) {
  size_t num_added = ParseData<std::string, std::string_view, ParseStringToken>(
      tokenizer, type_name, string_storage_);
  strings_.clear();
  for (size_t i = 0; i < num_added; i++) {
    strings_.emplace_back(string_storage_[i]);
  }
  return STRING;
}

ParameterList::Type ParameterList::ParseTexture(Tokenizer& tokenizer,
                                                std::string_view type_name,
                                                Color::Space rgb_color_space) {
  ParseString(tokenizer, type_name, rgb_color_space);
  return TEXTURE;
}

ParameterList::Type ParameterList::ParseVector3(Tokenizer& tokenizer,
                                                std::string_view type_name,
                                                Color::Space rgb_color_space) {
  ParseFloat(tokenizer, type_name, rgb_color_space);

  if (floats_.size() % 3 != 0) {
    std::cerr << "ERROR: The number of values in a " << type_name
              << " parameter list must be evenly divisible by 3" << std::endl;
    exit(EXIT_FAILURE);
  }

  vectors_.clear();
  for (size_t i = 0; i < floats_.size(); i += 3) {
    if (floats_[i] == 0.0 && floats_[i + 1] == 0.0 && floats_[i + 2] == 0.0) {
      std::cerr << "ERROR: A " << type_name
                << " parameter list must contain at least one non-zero value "
                   "for each output "
                << type_name << std::endl;
      exit(EXIT_FAILURE);
    }

    geometric x = static_cast<geometric>(floats_[i]);
    geometric y = static_cast<geometric>(floats_[i + 1]);
    geometric z = static_cast<geometric>(floats_[i + 2]);
    if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z)) {
      std::cerr << "ERROR: A " << type_name
                << " parameter list value was out of range" << std::endl;
      exit(EXIT_FAILURE);
    }

    vectors_.emplace_back(x, y, z);
  }

  return VECTOR3;
}

ParameterList::Type ParameterList::ParseXyz(Tokenizer& tokenizer,
                                            std::string_view type_name,
                                            Color::Space rgb_color_space) {
  ParseFloat(tokenizer, type_name, rgb_color_space);

  if (floats_.size() % 3 != 0) {
    std::cerr << "ERROR: The number of values in an " << type_name
              << " parameter list must be evenly divisible by 3" << std::endl;
    exit(EXIT_FAILURE);
  }

  colors_.clear();
  for (size_t i = 0; i < floats_.size(); i += 3) {
    if (floats_[i] < 0.0 || floats_[i + 1] < 0.0 || floats_[i + 2] < 0.0) {
      std::cerr << "ERROR: The values in an " << type_name
                << " parameter list cannot be negative" << std::endl;
      exit(EXIT_FAILURE);
    }

    visual_t x = static_cast<visual_t>(floats_[i]);
    visual_t y = static_cast<visual_t>(floats_[i + 1]);
    visual_t z = static_cast<visual_t>(floats_[i + 2]);
    if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z)) {
      std::cerr << "ERROR: A " << type_name
                << " parameter list value was out of range" << std::endl;
      exit(EXIT_FAILURE);
    }

    colors_.emplace_back(x, y, z, Color::CIE_XYZ);
  }

  return COLOR;
}

bool ParameterList::ParseFrom(Tokenizer& tokenizer,
                              Color::Space rgb_color_space) {
  auto type_and_name = ParseTypeAndName(tokenizer);
  if (!type_and_name) {
    return false;
  }

  name_storage_ = type_and_name->second;
  name_ = name_storage_;

  static const std::unordered_map<
      std::string_view,
      Type (ParameterList::*)(Tokenizer&, std::string_view, Color::Space)>
      functions = {
          // blackbody
          {"bool", &ParameterList::ParseBool},
          {"color", &ParameterList::ParseRgb},
          {"float", &ParameterList::ParseFloat},
          {"integer", &ParameterList::ParseInteger},
          {"normal", &ParameterList::ParseNormal},
          {"point", &ParameterList::ParsePoint3},
          // point2
          {"point3", &ParameterList::ParsePoint3},
          {"rgb", &ParameterList::ParseRgb},
          {"spectrum", &ParameterList::ParseSpectrum},
          {"string", &ParameterList::ParseString},
          {"texture", &ParameterList::ParseTexture},
          {"vector", &ParameterList::ParseVector3},
          // vector2
          {"vector3", &ParameterList::ParseVector3},
          {"xyz", &ParameterList::ParseXyz},
      };

  auto iter = functions.find(type_and_name->first);
  if (iter == functions.end()) {
    return false;
  }

  // Consume the peeked token
  tokenizer.Next();

  type_ =
      (this->*(iter->second))(tokenizer, type_and_name->first, rgb_color_space);

  return true;
}

}  // namespace iris::pbrt_frontend