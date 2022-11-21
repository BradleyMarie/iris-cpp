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
  if (file_path.is_relative()) {
    file_path = *tokenizers_.top().second / file_path;
  }

  std::filesystem::path search_path = file_path;
  search_path.remove_filename();

  auto file = std::make_unique<std::ifstream>(file_path);
  if (file->fail()) {
    std::cerr << "ERROR: Failed to open file:" << file_path << std::endl;
    exit(EXIT_FAILURE);
  }

  owned_tokenizers_.emplace(Tokenizer(*file), std::move(search_path),
                            std::move(file));

  tokenizers_.emplace(&std::get<0>(owned_tokenizers_.top()),
                      &std::get<1>(owned_tokenizers_.top()));
}

std::optional<std::string_view> Parser::NextToken() {
  while (!tokenizers_.empty()) {
    if (tokenizers_.top().first->Peek()) {
      return tokenizers_.top().first->Next();
    }

    tokenizers_.pop();
    if (!owned_tokenizers_.empty()) {
      owned_tokenizers_.pop();
    }
  }

  return std::nullopt;
}

std::optional<Renderable> Parser::ParseFrom(
    const std::filesystem::path& path_root, Tokenizer& tokenizer) {
  static const std::unordered_map<std::string_view, void (Parser::*)()>
      callbacks = {
          {"Include", &Parser::Include},
      };

  tokenizers_.emplace(&tokenizer, &path_root);
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