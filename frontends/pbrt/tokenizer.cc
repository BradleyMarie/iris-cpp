#include "frontends/pbrt/tokenizer.h"

#include <iostream>

namespace iris::pbrt_frontend {
namespace {

static char ReadEscapedCharacter(char ch) {
  switch (ch) {
    case 'b':
      return '\b';
    case 'f':
      return '\f';
    case 'n':
      return '\n';
    case 'r':
      return '\r';
    case 't':
      return '\t';
    case '\\':
      return '\\';
    case '\'':
      return '\'';
    case '"':
      return '"';
  }

  std::cerr << "ERROR: Illegal escape character: \\" << ch << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace

Tokenizer::Tokenizer(Tokenizer&& moved_from)
    : stream_(moved_from.stream_),
      next_(moved_from.next_),
      peeked_(moved_from.peeked_),
      peeked_valid_(moved_from.peeked_valid_) {
  moved_from.stream_ = nullptr;
  moved_from.next_.clear();
  moved_from.peeked_.clear();
  moved_from.peeked_valid_.reset();
}

Tokenizer& Tokenizer::operator=(Tokenizer&& moved_from) {
  stream_ = moved_from.stream_;
  next_ = moved_from.next_;
  peeked_ = moved_from.peeked_;
  peeked_valid_ = moved_from.peeked_valid_;
  moved_from.stream_ = nullptr;
  moved_from.next_.clear();
  moved_from.peeked_.clear();
  moved_from.peeked_valid_.reset();
  return *this;
}

bool Tokenizer::ParseNext(std::string& output) {
  output.clear();

  if (!stream_) {
    return false;
  }

  for (int read = stream_->get(); read != EOF; read = stream_->get()) {
    if (std::isspace(read)) {
      continue;
    }

    char ch = static_cast<char>(read);
    if (ch == '"') {
      output.push_back(ch);
      bool just_escaped = false;
      bool found_end = false;

      for (read = stream_->get(); read != EOF; read = stream_->get()) {
        ch = static_cast<char>(read);

        if (ch == '\n') {
          std::cerr << "ERROR: New line found before end of quoted string"
                    << std::endl;
          exit(EXIT_FAILURE);
        }

        if (just_escaped) {
          ch = ReadEscapedCharacter(ch);
          just_escaped = false;
        } else if (ch == '\\') {
          just_escaped = true;
          continue;
        } else if (ch == '"') {
          found_end = true;
        }

        output.push_back(ch);
      }

      if (just_escaped) {
        std::cerr << "ERROR: A token may not end with the escape character"
                  << std::endl;
        exit(EXIT_FAILURE);
      }

      if (!found_end) {
        std::cerr << "ERROR: Unterminated quoted string" << std::endl;
        exit(EXIT_FAILURE);
      }

      return true;
    }

    if (ch == '[' || ch == ']') {
      output.push_back(ch);
      return true;
    }

    if (ch == '#') {
      for (read = stream_->get(); read != EOF; read = stream_->get()) {
        ch = static_cast<char>(read);
        if (ch == '\r' || ch == '\n') {
          break;
        }
      }

      continue;
    }

    output.push_back(ch);
    for (int peeked = stream_->peek(); peeked != EOF;
         peeked = stream_->peek()) {
      if (std::isspace(peeked) || peeked == '"' || peeked == '[' ||
          peeked == ']') {
        break;
      }

      *stream_ >> ch;
      output.push_back(ch);
    }

    return true;
  }

  return false;
}

std::optional<std::string_view> Tokenizer::Peek() {
  if (peeked_valid_.has_value()) {
    if (*peeked_valid_) {
      return peeked_;
    } else {
      return std::nullopt;
    }
  }

  bool found = ParseNext(peeked_);
  if (found) {
    peeked_valid_ = found;
    return peeked_;
  }

  return std::nullopt;
}

std::optional<std::string_view> Tokenizer::Next() {
  bool next_valid;
  if (peeked_valid_.has_value()) {
    std::swap(next_, peeked_);
    next_valid = *peeked_valid_;
    peeked_valid_ = std::nullopt;
  } else {
    next_valid = ParseNext(next_);
  }

  if (next_valid) {
    return next_;
  }

  return std::nullopt;
}

}  // namespace iris::pbrt_frontend