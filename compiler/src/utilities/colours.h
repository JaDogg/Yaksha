// colors.h
// Based on Color Terminal Output Library -- MIT Licensed Library by Ph0enixKM @ GitHub
// Bhathiya Perera - did some heavy modifications to the original library
#ifndef COLORS_H
#define COLORS_H
#include <iostream>
#include <string>
namespace yaksha {
  enum class colour {
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    NORMAL [[maybe_unused]]
  };
  bool is_stdout_a_terminal();
  void colour_print(colour c, const std::string &content, std::ostream &out);
  struct coloured_content {
    coloured_content(colour c, const std::string &content)
        : c_(c), line_(content) {}
    friend std::ostream &operator<<(std::ostream &outs,
                                    const coloured_content &p);

private:
    colour c_;
    const std::string &line_;
  };
  std::ostream &operator<<(std::ostream &outs, const coloured_content &p);
  namespace colours {
    static inline coloured_content red(const std::string &content) {
      return {colour::RED, content};
    }
    static inline coloured_content green(const std::string &content) {
      return {colour::GREEN, content};
    }
    static inline coloured_content yellow(const std::string &content) {
      return {colour::YELLOW, content};
    }
    static inline coloured_content blue(const std::string &content) {
      return {colour::BLUE, content};
    }
    static inline coloured_content magenta(const std::string &content) {
      return {colour::MAGENTA, content};
    }
    static inline coloured_content cyan(const std::string &content) {
      return {colour::CYAN, content};
    }
    static inline coloured_content normal(const std::string &content) {
      return {colour::NORMAL, content};
    }
  }// namespace colours
}// namespace yaksha
#endif
