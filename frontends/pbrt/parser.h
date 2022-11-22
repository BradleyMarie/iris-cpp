#ifndef _FRONTENDS_PBRT_PARSER_
#define _FRONTENDS_PBRT_PARSER_

#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "frontends/pbrt/renderable.h"
#include "frontends/pbrt/tokenizer.h"

namespace iris::pbrt_frontend {

class Parser {
 public:
  std::optional<Renderable> ParseFrom(
      const std::filesystem::path& search_root, Tokenizer& tokenizer,
      std::optional<std::filesystem::path> file_path = std::nullopt);

 private:
  std::optional<std::string_view> NextToken();

  void Include();

  struct TokenizerEntry {
    Tokenizer* tokenizer;
    std::unique_ptr<Tokenizer> owned_tokenizer;
    std::filesystem::path search_path;
    std::optional<std::filesystem::path> file_path;
    std::unique_ptr<std::ifstream> file;
  };

  std::vector<TokenizerEntry> tokenizers_;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_PARSER_