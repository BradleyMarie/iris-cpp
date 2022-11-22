#include "frontends/pbrt/parser.h"

#include <cstdlib>
#include <iostream>
#include <unordered_map>

#include "frontends/pbrt/quoted_string.h"

namespace iris::pbrt_frontend {

void Parser::Include() {
  auto next = NextToken();
  if (!next) {
    std::cerr << "ERROR: Too few parameters to directive Include" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = Unquote(*next);
  if (!unquoted) {
    std::cerr << "ERROR: Parameter to Include must be a string" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::filesystem::path file_path(*unquoted);
  file_path = std::filesystem::weakly_canonical(file_path);

  for (const auto& entry : tokenizers_) {
    if (entry.file_path && file_path == *entry.file_path) {
      std::cerr << "ERROR: Detected cyclic Include of file: " << *unquoted
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  std::filesystem::path search_path = file_path;
  search_path.remove_filename();

  auto file = std::make_unique<std::ifstream>(file_path);
  if (file->fail()) {
    std::cerr << "ERROR: Failed to open file:" << *unquoted << std::endl;
    exit(EXIT_FAILURE);
  }

  auto tokenizer = std::make_unique<Tokenizer>(*file);

  tokenizers_.emplace_back(tokenizer.get(), std::move(tokenizer),
                           std::filesystem::weakly_canonical(search_path),
                           file_path, std::move(file));
}

std::optional<std::string_view> Parser::NextToken() {
  while (!tokenizers_.empty()) {
    if (tokenizers_.back().tokenizer->Peek()) {
      return tokenizers_.back().tokenizer->Next();
    }

    tokenizers_.pop_back();
  }

  return std::nullopt;
}

std::optional<Renderable> Parser::ParseFrom(
    const std::filesystem::path& search_root, Tokenizer& tokenizer,
    std::optional<std::filesystem::path> file_path) {
  static const std::unordered_map<std::string_view, void (Parser::*)()>
      callbacks = {
          {"Include", &Parser::Include},
      };

  if (file_path) {
    *file_path = std::filesystem::weakly_canonical(*file_path);
  }

  tokenizers_.emplace_back(&tokenizer, nullptr,
                           std::filesystem::weakly_canonical(search_root),
                           file_path, nullptr);

  for (;;) {
    auto token = NextToken();

    auto iter = callbacks.find(*token);
    if (iter == callbacks.end()) {
      std::cerr << "ERROR: Invalid directive: " << *token << std::endl;
      exit(EXIT_FAILURE);
    }

    (this->*iter->second)();
  }
}

}  // namespace iris::pbrt_frontend