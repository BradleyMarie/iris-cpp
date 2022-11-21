#ifndef _FRONTENDS_PBRT_PARSER_
#define _FRONTENDS_PBRT_PARSER_

#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <stack>
#include <string_view>
#include <tuple>
#include <utility>

#include "frontends/pbrt/renderable.h"
#include "frontends/pbrt/tokenizer.h"

namespace iris::pbrt_frontend {

class Parser {
 public:
  std::optional<Renderable> ParseFrom(const std::filesystem::path& path_root,
                                      Tokenizer& tokenizer);

 private:
  std::optional<std::string_view> NextToken();

  void Include();

  std::stack<std::pair<Tokenizer*, const std::filesystem::path*>> tokenizers_;
  std::stack<std::tuple<Tokenizer, std::filesystem::path,
                        std::unique_ptr<std::ifstream>>>
      owned_tokenizers_;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_PARSER_