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
    AND,
    AND_EQ,
    ARROW,
    AT,
    BA_DEDENT,
    BA_INDENT,
    COLON,
    COMMA,
    COMMENT,
    CURLY_BRACKET_CLOSE,
    CURLY_BRACKET_OPEN,
    DIV,
    DIV_EQ,
    DOT,
    ELLIPSIS,
    END_OF_FILE,
    EQ,
    EQ_EQ,
    FLOAT_NUMBER,
    GREAT,
    GREAT_EQ,
    INDENT,
    INTEGER_BIN,
    INTEGER_DECIMAL,
    INTEGER_HEX,
    INTEGER_OCT,
    INT_DIV,
    INT_DIV_EQ,
    LESS,
    LESS_EQ,
    MOD,
    MOD_EQ,
    MUL,
    MUL_EQ,
    NAME,
    NEW_LINE,
    NOT_EQ,
    OR,
    OR_EQ,
    PAREN_CLOSE,
    PAREN_OPEN,
    PLUS,
    PLUS_EQ,
    POWER,
    POWER_EQ,
    SHL,
    SHR,
    SQUARE_BRACKET_CLOSE,
    SQUARE_BRACKET_OPEN,
    STRING,
    SUB,
    SUB_EQ,
    THREE_QUOTE_STRING,
    TILDE,
    XOR,
    XOR_EQ,
    KEYWORD_FALSE,
    KEYWORD_NONE,
    KEYWORD_TRUE,
    KEYWORD_AND,
    KEYWORD_AS,
    KEYWORD_ASSERT,
    KEYWORD_BREAK,
    KEYWORD_CCODE,
    KEYWORD_CLASS,
    KEYWORD_CONTINUE,
    KEYWORD_DEF,
    KEYWORD_DEFER,
    KEYWORD_DEL,
    KEYWORD_ELIF,
    KEYWORD_ELSE,
    KEYWORD_FOR,
    KEYWORD_FROM,
    KEYWORD_IF,
    KEYWORD_IMPORT,
    KEYWORD_NOT,
    KEYWORD_OR,
    KEYWORD_PASS,
    KEYWORD_RETURN,
    KEYWORD_TRY,
    KEYWORD_WHILE,
    TK_UNKNOWN_TOKEN_DETECTED
  };
  /**
 * Convert a token to a string
 * @param t token type
 * @return token type as a std::string
 */
  static inline std::string token_to_str(const token_type &t) {
    if (t == token_type::AND) return "AND";
    if (t == token_type::AND_EQ) return "AND_EQ";
    if (t == token_type::ARROW) return "ARROW";
    if (t == token_type::AT) return "AT";
    if (t == token_type::BA_DEDENT) return "BA_DEDENT";
    if (t == token_type::BA_INDENT) return "BA_INDENT";
    if (t == token_type::COLON) return "COLON";
    if (t == token_type::COMMA) return "COMMA";
    if (t == token_type::COMMENT) return "COMMENT";
    if (t == token_type::CURLY_BRACKET_CLOSE) return "CURLY_BRACKET_CLOSE";
    if (t == token_type::CURLY_BRACKET_OPEN) return "CURLY_BRACKET_OPEN";
    if (t == token_type::DIV) return "DIV";
    if (t == token_type::DIV_EQ) return "DIV_EQ";
    if (t == token_type::DOT) return "DOT";
    if (t == token_type::ELLIPSIS) return "ELLIPSIS";
    if (t == token_type::END_OF_FILE) return "END_OF_FILE";
    if (t == token_type::EQ) return "EQ";
    if (t == token_type::EQ_EQ) return "EQ_EQ";
    if (t == token_type::FLOAT_NUMBER) return "FLOAT_NUMBER";
    if (t == token_type::GREAT) return "GREAT";
    if (t == token_type::GREAT_EQ) return "GREAT_EQ";
    if (t == token_type::INDENT) return "INDENT";
    if (t == token_type::INTEGER_BIN) return "INTEGER_BIN";
    if (t == token_type::INTEGER_DECIMAL) return "INTEGER_DECIMAL";
    if (t == token_type::INTEGER_HEX) return "INTEGER_HEX";
    if (t == token_type::INTEGER_OCT) return "INTEGER_OCT";
    if (t == token_type::INT_DIV) return "INT_DIV";
    if (t == token_type::INT_DIV_EQ) return "INT_DIV_EQ";
    if (t == token_type::LESS) return "LESS";
    if (t == token_type::LESS_EQ) return "LESS_EQ";
    if (t == token_type::MOD) return "MOD";
    if (t == token_type::MOD_EQ) return "MOD_EQ";
    if (t == token_type::MUL) return "MUL";
    if (t == token_type::MUL_EQ) return "MUL_EQ";
    if (t == token_type::NAME) return "NAME";
    if (t == token_type::NEW_LINE) return "NEW_LINE";
    if (t == token_type::NOT_EQ) return "NOT_EQ";
    if (t == token_type::OR) return "OR";
    if (t == token_type::OR_EQ) return "OR_EQ";
    if (t == token_type::PAREN_CLOSE) return "PAREN_CLOSE";
    if (t == token_type::PAREN_OPEN) return "PAREN_OPEN";
    if (t == token_type::PLUS) return "PLUS";
    if (t == token_type::PLUS_EQ) return "PLUS_EQ";
    if (t == token_type::POWER) return "POWER";
    if (t == token_type::POWER_EQ) return "POWER_EQ";
    if (t == token_type::SHL) return "SHL";
    if (t == token_type::SHR) return "SHR";
    if (t == token_type::SQUARE_BRACKET_CLOSE) return "SQUARE_BRACKET_CLOSE";
    if (t == token_type::SQUARE_BRACKET_OPEN) return "SQUARE_BRACKET_OPEN";
    if (t == token_type::STRING) return "STRING";
    if (t == token_type::SUB) return "SUB";
    if (t == token_type::SUB_EQ) return "SUB_EQ";
    if (t == token_type::THREE_QUOTE_STRING) return "THREE_QUOTE_STRING";
    if (t == token_type::TILDE) return "TILDE";
    if (t == token_type::XOR) return "XOR";
    if (t == token_type::XOR_EQ) return "XOR_EQ";
    if (t == token_type::KEYWORD_FALSE) return "KEYWORD_FALSE";
    if (t == token_type::KEYWORD_NONE) return "KEYWORD_NONE";
    if (t == token_type::KEYWORD_TRUE) return "KEYWORD_TRUE";
    if (t == token_type::KEYWORD_AND) return "KEYWORD_AND";
    if (t == token_type::KEYWORD_AS) return "KEYWORD_AS";
    if (t == token_type::KEYWORD_ASSERT) return "KEYWORD_ASSERT";
    if (t == token_type::KEYWORD_BREAK) return "KEYWORD_BREAK";
    if (t == token_type::KEYWORD_CCODE) return "KEYWORD_CCODE";
    if (t == token_type::KEYWORD_CLASS) return "KEYWORD_CLASS";
    if (t == token_type::KEYWORD_CONTINUE) return "KEYWORD_CONTINUE";
    if (t == token_type::KEYWORD_DEF) return "KEYWORD_DEF";
    if (t == token_type::KEYWORD_DEFER) return "KEYWORD_DEFER";
    if (t == token_type::KEYWORD_DEL) return "KEYWORD_DEL";
    if (t == token_type::KEYWORD_ELIF) return "KEYWORD_ELIF";
    if (t == token_type::KEYWORD_ELSE) return "KEYWORD_ELSE";
    if (t == token_type::KEYWORD_FOR) return "KEYWORD_FOR";
    if (t == token_type::KEYWORD_FROM) return "KEYWORD_FROM";
    if (t == token_type::KEYWORD_IF) return "KEYWORD_IF";
    if (t == token_type::KEYWORD_IMPORT) return "KEYWORD_IMPORT";
    if (t == token_type::KEYWORD_NOT) return "KEYWORD_NOT";
    if (t == token_type::KEYWORD_OR) return "KEYWORD_OR";
    if (t == token_type::KEYWORD_PASS) return "KEYWORD_PASS";
    if (t == token_type::KEYWORD_RETURN) return "KEYWORD_RETURN";
    if (t == token_type::KEYWORD_TRY) return "KEYWORD_TRY";
    if (t == token_type::KEYWORD_WHILE) return "KEYWORD_WHILE";
    return "TK_UNKNOWN_TOKEN_DETECTED";
  }
  static inline token_type str_to_token(const std::string &t) {
    if (t == "AND") return token_type::AND;
    if (t == "AND_EQ") return token_type::AND_EQ;
    if (t == "ARROW") return token_type::ARROW;
    if (t == "AT") return token_type::AT;
    if (t == "BA_DEDENT") return token_type::BA_DEDENT;
    if (t == "BA_INDENT") return token_type::BA_INDENT;
    if (t == "COLON") return token_type::COLON;
    if (t == "COMMA") return token_type::COMMA;
    if (t == "COMMENT") return token_type::COMMENT;
    if (t == "CURLY_BRACKET_CLOSE") return token_type::CURLY_BRACKET_CLOSE;
    if (t == "CURLY_BRACKET_OPEN") return token_type::CURLY_BRACKET_OPEN;
    if (t == "DIV") return token_type::DIV;
    if (t == "DIV_EQ") return token_type::DIV_EQ;
    if (t == "DOT") return token_type::DOT;
    if (t == "ELLIPSIS") return token_type::ELLIPSIS;
    if (t == "END_OF_FILE") return token_type::END_OF_FILE;
    if (t == "EQ") return token_type::EQ;
    if (t == "EQ_EQ") return token_type::EQ_EQ;
    if (t == "FLOAT_NUMBER") return token_type::FLOAT_NUMBER;
    if (t == "GREAT") return token_type::GREAT;
    if (t == "GREAT_EQ") return token_type::GREAT_EQ;
    if (t == "INDENT") return token_type::INDENT;
    if (t == "INTEGER_BIN") return token_type::INTEGER_BIN;
    if (t == "INTEGER_DECIMAL") return token_type::INTEGER_DECIMAL;
    if (t == "INTEGER_HEX") return token_type::INTEGER_HEX;
    if (t == "INTEGER_OCT") return token_type::INTEGER_OCT;
    if (t == "INT_DIV") return token_type::INT_DIV;
    if (t == "INT_DIV_EQ") return token_type::INT_DIV_EQ;
    if (t == "LESS") return token_type::LESS;
    if (t == "LESS_EQ") return token_type::LESS_EQ;
    if (t == "MOD") return token_type::MOD;
    if (t == "MOD_EQ") return token_type::MOD_EQ;
    if (t == "MUL") return token_type::MUL;
    if (t == "MUL_EQ") return token_type::MUL_EQ;
    if (t == "NAME") return token_type::NAME;
    if (t == "NEW_LINE") return token_type::NEW_LINE;
    if (t == "NOT_EQ") return token_type::NOT_EQ;
    if (t == "OR") return token_type::OR;
    if (t == "OR_EQ") return token_type::OR_EQ;
    if (t == "PAREN_CLOSE") return token_type::PAREN_CLOSE;
    if (t == "PAREN_OPEN") return token_type::PAREN_OPEN;
    if (t == "PLUS") return token_type::PLUS;
    if (t == "PLUS_EQ") return token_type::PLUS_EQ;
    if (t == "POWER") return token_type::POWER;
    if (t == "POWER_EQ") return token_type::POWER_EQ;
    if (t == "SHL") return token_type::SHL;
    if (t == "SHR") return token_type::SHR;
    if (t == "SQUARE_BRACKET_CLOSE") return token_type::SQUARE_BRACKET_CLOSE;
    if (t == "SQUARE_BRACKET_OPEN") return token_type::SQUARE_BRACKET_OPEN;
    if (t == "STRING") return token_type::STRING;
    if (t == "SUB") return token_type::SUB;
    if (t == "SUB_EQ") return token_type::SUB_EQ;
    if (t == "THREE_QUOTE_STRING") return token_type::THREE_QUOTE_STRING;
    if (t == "TILDE") return token_type::TILDE;
    if (t == "XOR") return token_type::XOR;
    if (t == "XOR_EQ") return token_type::XOR_EQ;
    if (t == "KEYWORD_FALSE") return token_type::KEYWORD_FALSE;
    if (t == "KEYWORD_NONE") return token_type::KEYWORD_NONE;
    if (t == "KEYWORD_TRUE") return token_type::KEYWORD_TRUE;
    if (t == "KEYWORD_AND") return token_type::KEYWORD_AND;
    if (t == "KEYWORD_AS") return token_type::KEYWORD_AS;
    if (t == "KEYWORD_ASSERT") return token_type::KEYWORD_ASSERT;
    if (t == "KEYWORD_BREAK") return token_type::KEYWORD_BREAK;
    if (t == "KEYWORD_CCODE") return token_type::KEYWORD_CCODE;
    if (t == "KEYWORD_CLASS") return token_type::KEYWORD_CLASS;
    if (t == "KEYWORD_CONTINUE") return token_type::KEYWORD_CONTINUE;
    if (t == "KEYWORD_DEF") return token_type::KEYWORD_DEF;
    if (t == "KEYWORD_DEFER") return token_type::KEYWORD_DEFER;
    if (t == "KEYWORD_DEL") return token_type::KEYWORD_DEL;
    if (t == "KEYWORD_ELIF") return token_type::KEYWORD_ELIF;
    if (t == "KEYWORD_ELSE") return token_type::KEYWORD_ELSE;
    if (t == "KEYWORD_FOR") return token_type::KEYWORD_FOR;
    if (t == "KEYWORD_FROM") return token_type::KEYWORD_FROM;
    if (t == "KEYWORD_IF") return token_type::KEYWORD_IF;
    if (t == "KEYWORD_IMPORT") return token_type::KEYWORD_IMPORT;
    if (t == "KEYWORD_NOT") return token_type::KEYWORD_NOT;
    if (t == "KEYWORD_OR") return token_type::KEYWORD_OR;
    if (t == "KEYWORD_PASS") return token_type::KEYWORD_PASS;
    if (t == "KEYWORD_RETURN") return token_type::KEYWORD_RETURN;
    if (t == "KEYWORD_TRY") return token_type::KEYWORD_TRY;
    if (t == "KEYWORD_WHILE") return token_type::KEYWORD_WHILE;
    return token_type::TK_UNKNOWN_TOKEN_DETECTED;
  }
  static inline token_type str_to_keyword(const std::string &t) {
    if (t == "False") return token_type::KEYWORD_FALSE;
    if (t == "None") return token_type::KEYWORD_NONE;
    if (t == "True") return token_type::KEYWORD_TRUE;
    if (t == "and") return token_type::KEYWORD_AND;
    if (t == "as") return token_type::KEYWORD_AS;
    if (t == "assert") return token_type::KEYWORD_ASSERT;
    if (t == "break") return token_type::KEYWORD_BREAK;
    if (t == "ccode") return token_type::KEYWORD_CCODE;
    if (t == "class") return token_type::KEYWORD_CLASS;
    if (t == "continue") return token_type::KEYWORD_CONTINUE;
    if (t == "def") return token_type::KEYWORD_DEF;
    if (t == "defer") return token_type::KEYWORD_DEFER;
    if (t == "del") return token_type::KEYWORD_DEL;
    if (t == "elif") return token_type::KEYWORD_ELIF;
    if (t == "else") return token_type::KEYWORD_ELSE;
    if (t == "for") return token_type::KEYWORD_FOR;
    if (t == "from") return token_type::KEYWORD_FROM;
    if (t == "if") return token_type::KEYWORD_IF;
    if (t == "import") return token_type::KEYWORD_IMPORT;
    if (t == "not") return token_type::KEYWORD_NOT;
    if (t == "or") return token_type::KEYWORD_OR;
    if (t == "pass") return token_type::KEYWORD_PASS;
    if (t == "return") return token_type::KEYWORD_RETURN;
    if (t == "try") return token_type::KEYWORD_TRY;
    if (t == "while") return token_type::KEYWORD_WHILE;
    return token_type::TK_UNKNOWN_TOKEN_DETECTED;
  }
  /**
 * Single token
 */
  struct token {
    std::string file_; // filename
    int line_;         // line in file
    int pos_;          // position in line
    std::string token_;// token as a bunch of uint32_t characters
    token_type type_;  // type of the token
  };
  /**
 * Content of an error message
 */
  struct parsing_error {
    std::string message_;// content of the line this error occurred
    token tok_;
    bool token_set_{};
    parsing_error(std::string message, token *at);
    parsing_error(std::string message, std::string file, int line, int pos);
  };
}// namespace yaksha
#endif