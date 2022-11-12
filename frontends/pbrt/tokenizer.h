#ifndef _FRONTENDS_PBRT_TOKENIZER_
#define _FRONTENDS_PBRT_TOKENIZER_

#include <istream>
#include <optional>
#include <string_view>

namespace iris::pbrt_frontend {

class Tokenizer {
 public:
  Tokenizer(std::istream& stream) noexcept : stream_(&stream) {}

  Tokenizer(Tokenizer&& moved_from);
  Tokenizer& operator=(Tokenizer&& moved_from);

  Tokenizer(const Tokenizer&) = delete;
  Tokenizer& operator=(const Tokenizer&) = delete;

  std::optional<std::string_view> Peek();
  std::optional<std::string_view> Next();

 private:
  bool ParseNext(std::string& output);

  std::istream* stream_;
  std::string next_;
  std::string peeked_;
  std::optional<bool> peeked_valid_;
};

}  // namespace iris::pbrt_frontend

#endif  // _IRIS_FRONTEND_TOKENIZER_