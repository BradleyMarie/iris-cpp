#ifndef _FRONTENDS_PBRT_PARAMETER_LIST_
#define _FRONTENDS_PBRT_PARAMETER_LIST_

#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "frontends/pbrt/tokenizer.h"
#include "iris/color.h"
#include "iris/point.h"
#include "iris/vector.h"

namespace iris::pbrt_frontend {

class ParameterList {
 public:
  enum Type {
    // BLACKBODY,
    BOOL,
    COLOR,
    FLOAT,
    INTEGER,
    NORMAL,
    // POINT2,
    POINT3,
    SPECTRUM,
    STRING,
    TEXTURE,
    // VECTOR2,
    VECTOR3,
  };

  std::string_view GetName() const;
  std::string_view GetTypeName() const;
  Type GetType() const;

  const std::vector<bool>& GetBoolValues() const;
  const std::vector<Color> GetColorValues() const;
  const std::vector<long double>& GetFloatValues() const;
  const std::vector<int64_t>& GetIntegerValues() const;
  const std::vector<Vector>& GetNormalValues() const;
  const std::vector<Point>& GetPoint3Values() const;
  const std::map<visual, visual>& GetSpectrumValues() const;
  const std::vector<std::string_view>& GetStringValues() const;
  const std::vector<std::string_view>& GetTextureValues() const;
  const std::vector<Vector>& GetVector3Values() const;

  bool ParseFrom(Tokenizer& tokenizer, Color::Space rgb_color_space);

 private:
  Type ParseBool(Tokenizer& tokenizer, std::string_view type_name,
                 Color::Space rgb_color_space);
  Type ParseFloat(Tokenizer& tokenizer, std::string_view type_name,
                  Color::Space rgb_color_space);
  Type ParseInteger(Tokenizer& tokenizer, std::string_view type_name,
                    Color::Space rgb_color_space);
  Type ParseNormal(Tokenizer& tokenizer, std::string_view type_name,
                   Color::Space rgb_color_space);
  Type ParsePoint3(Tokenizer& tokenizer, std::string_view type_name,
                   Color::Space rgb_color_space);
  Type ParseRgb(Tokenizer& tokenizer, std::string_view type_name,
                Color::Space rgb_color_space);
  Type ParseSpectrum(Tokenizer& tokenizer, std::string_view type_name,
                     Color::Space rgb_color_space);
  Type ParseString(Tokenizer& tokenizer, std::string_view type_name,
                   Color::Space rgb_color_space);
  Type ParseTexture(Tokenizer& tokenizer, std::string_view type_name,
                    Color::Space rgb_color_space);
  Type ParseVector3(Tokenizer& tokenizer, std::string_view type_name,
                    Color::Space rgb_color_space);
  Type ParseXyz(Tokenizer& tokenizer, std::string_view type_name,
                Color::Space rgb_color_space);

  std::optional<std::string_view> name_;
  std::string name_storage_;
  std::optional<std::string_view> type_name_;
  std::optional<Type> type_;
  std::vector<bool> bools_;
  std::vector<Color> colors_;
  std::vector<long double> floats_;
  std::vector<int64_t> integers_;
  std::map<visual, visual> spectrum_;
  std::vector<std::string_view> strings_;
  std::vector<std::string> string_storage_;
  std::vector<Point> points_;
  std::vector<Vector> vectors_;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_PARAMETER_LIST_