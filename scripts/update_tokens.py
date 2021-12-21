#!/usr/bin/env python
"""
Update token.h
Generate utility functions and enums for different types of tokens
Keyword token related functions are generated as well
Template also contains - `token` & `parsing_error` data structures.
"""
import os.path

K_TOKENS_LIST = "$TOKENS$"
K_TOKEN_TO_STR = "$TOKEN_TO_STR$"
K_STR_TO_TOKEN = "$STR_TO_TOKEN$"
K_KW_TO_TOKEN = "$KW_TO_TOKEN$"
UNKNOWN = "TK_UNKNOWN_TOKEN_DETECTED"
RESERVED = ["await", "except", "raise", "finally", "lambda", "nonlocal", "global", "with", "async", "yield", "in", "is"]
KEYWORDS = sorted(["False", "else", "import", "pass",
                   "None", "break",
                   "True", "class", "return",
                   "and", "continue", "for", "try",
                   "as", "def", "from", "while",
                   "assert", "del", "not",
                   "elif", "if", "or", "print"])
TOKENS = sorted([
    "NAME", "AT", "INTEGER_DECIMAL", "INTEGER_OCT", "INTEGER_HEX", "INTEGER_BIN", "FLOAT_NUMBER", "INDENT",
    "BA_INDENT", "BA_DEDENT", "NEW_LINE", "COLON", "COMMENT", "THREE_QUOTE_STRING", "STRING", "PAREN_OPEN",
    "PAREN_CLOSE", "SQUARE_BRACKET_OPEN", "SQUARE_BRACKET_CLOSE", "CURLY_BRACKET_OPEN", "CURLY_BRACKET_CLOSE", "LESS",
    "EQ", "GREAT", "OR", "PLUS", "SUB", "MUL", "DIV", "AND", "XOR", "MOD", "INT_DIV", "POWER", "LESS_EQ", "EQ_EQ",
    "GREAT_EQ", "OR_EQ", "PLUS_EQ", "SUB_EQ", "MUL_EQ", "DIV_EQ", "AND_EQ", "XOR_EQ", "MOD_EQ", "INT_DIV_EQ",
    "POWER_EQ", "ARROW", "ELLIPSIS", "DOT", "COMMA", "SHL", "SHR", "NOT_EQ", "TILDE", "END_OF_FILE"
]) + ["KEYWORD_" + x.upper() for x in KEYWORDS]
TEMPLATE = r"""
// token.h
// generated by update_tokens.py
#ifndef TOKEN_H
#define TOKEN_H
#include <string>
namespace yaksha {
/**
 * Most simple types of tokens we can match with tokenizing
 */
enum class token_type {
$TOKENS$
};
/**
 * Convert a token to a string
 * @param t token type
 * @return token type as a std::string
 */
static inline std::string token_to_str(const token_type &t) {
$TOKEN_TO_STR$
}
static inline token_type str_to_token(const std::string &t) {
$STR_TO_TOKEN$
}
static inline token_type str_to_keyword(const std::string &t) {
$KW_TO_TOKEN$
}
/**
 * Single token
 */
struct token {
  std::string file_;  // filename
  int line_;          // line in file
  int pos_;           // position in line
  std::string token_; // token as a bunch of uint32_t characters
  token_type type_;   // type of the token
};
/**
 * Content of an error message
 */
struct parsing_error {
  std::string message_; // content of the line this error occurred
  token tok_;
  bool token_set_{};
  parsing_error(std::string message, token* at);
  parsing_error(std::string message, std::string file, int line, int pos);
};
} // namespace yaksha
#endif
""".strip()
STR_TOK_TEMPLATE = r"""
  if (t == "$K$")
    return token_type::$K$;
""".strip()
TOK_STR_TEMPLATE = r"""
  if (t == token_type::$K$)
    return "$K$";
""".strip()
KW_TO_TOKEN = r"""
  if (t == "$K$")
    return token_type::KEYWORD_$KU$;
""".strip()

if __name__ == "__main__":
    os.chdir(os.path.dirname(os.path.dirname(__file__)))
    with open(os.path.join(".", "src", "tokenizer", "token.h"), "w+") as h:
        token_list = ",\n".join(["  " + x for x in TOKENS]) + ",\n  " + UNKNOWN
        str_to_tok = "\n".join(["  " + STR_TOK_TEMPLATE.replace("$K$", x) for x in TOKENS]) \
                     + "\n  return token_type::" + UNKNOWN + ";"
        kw_to_tok = "\n".join(["  " + KW_TO_TOKEN.replace("$K$", x).replace("$KU$", x.upper()) for x in KEYWORDS]) \
                    + "\n  return token_type::" + UNKNOWN + ";"
        tok_to_str = "\n".join(["  " + TOK_STR_TEMPLATE.replace("$K$", x) for x in TOKENS]) \
                     + "\n  return \"" + UNKNOWN + "\";"
        code = TEMPLATE.replace(K_TOKENS_LIST, token_list) \
            .replace(K_TOKEN_TO_STR, tok_to_str).replace(K_STR_TO_TOKEN, str_to_tok) \
            .replace(K_KW_TO_TOKEN, kw_to_tok)
        h.write(code)
