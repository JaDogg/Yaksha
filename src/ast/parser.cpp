// parser.cpp
#include "ast/parser.h"
#include <algorithm>
#include <cassert>
using namespace yaksha;
parser::parser(std::vector<token> &tokens)
    : pool_{}, tokens_{tokens}, current_{0} {}
token *parser::advance() {
  if (!is_at_end()) { current_++; }
  return previous();
}
token *parser::recede() {
  if (current_ != 0) { current_--; }
  return peek();
}
bool parser::is_at_end() { return peek()->type_ == token_type::END_OF_FILE; }
token *parser::peek() { return &tokens_[current_]; }
token *parser::previous() {
  assert(current_ > 0);
  return &tokens_[current_ - 1];
}
token *parser::look_ahead() {
  if (is_at_end()) { return peek(); }
  return &tokens_[current_ + 1];
}
bool parser::match(std::initializer_list<token_type> types) {
  return std::any_of(types.begin(), types.end(), [this](token_type t) {
    if (this->check(t)) {
      this->advance();
      return true;
    }
    return false;
  });
}
bool parser::check(token_type t) {
  if (is_at_end()) { return false; }
  return peek()->type_ == t;
}
expr *parser::equality() {
  expr *ex = comparison();
  while (match({token_type::NOT_EQ, token_type::EQ_EQ})) {
    auto opr = previous();
    expr *right = comparison();
    ex = pool_.c_binary_expr(ex, opr, right);
  }
  return ex;
}
expr *parser::comparison() {
  expr *ex = term();
  while (match({token_type::GREAT, token_type::GREAT_EQ, token_type::LESS,
                token_type::LESS_EQ})) {
    auto opr = previous();
    expr *right = term();
    ex = pool_.c_binary_expr(ex, opr, right);
  }
  return ex;
}
expr *parser::term() {
  expr *ex = factor();
  while (match({token_type::SUB, token_type::PLUS})) {
    auto opr = previous();
    expr *right = factor();
    ex = pool_.c_binary_expr(ex, opr, right);
  }
  return ex;
}
expr *parser::factor() {
  expr *ex = unary();
  while (match({token_type::DIV, token_type::MUL, token_type::INT_DIV})) {
    auto opr = previous();
    expr *right = unary();
    ex = pool_.c_binary_expr(ex, opr, right);
  }
  return ex;
}
expr *parser::unary() {
  if (match({token_type::SUB, token_type::KEYWORD_NOT})) {
    auto opr = previous();
    expr *right = unary();
    return pool_.c_unary_expr(opr, right);
  }
  return primary();
}
expr *parser::primary() {
  if (match({token_type::KEYWORD_FALSE, token_type::KEYWORD_TRUE,
             token_type::KEYWORD_NONE, token_type::FLOAT_NUMBER,
             token_type::INTEGER_DECIMAL, token_type::INTEGER_BIN,
             token_type::INTEGER_OCT, token_type::INTEGER_HEX,
             token_type::STRING})) {
    return pool_.c_literal_expr(previous());
  }
  if (match({token_type::NAME})) { return pool_.c_variable_expr(previous()); }
  if (match({token_type::PAREN_OPEN})) {
    expr *ex = expression();
    consume(token_type::PAREN_CLOSE, "Expect ')' after expression");
    return pool_.c_grouping_expr(ex);
  }
  throw error(peek(), "Expected expression!");
}
token *parser::consume(token_type t, const std::string &message) {
  if (check(t)) return advance();
  throw error(peek(), message);
}
parsing_error parser::error(token *tok, const std::string &message) {
  auto err = parsing_error{message, tok};
  handle_error(err);
  return err;
}
void parser::handle_error(const parsing_error &err) { errors_.push_back(err); }
expr *parser::expression() { return assignment(); }
std::vector<stmt *> parser::parse() {
  std::vector<stmt *> statements{};
  while (!is_at_end()) {
    auto decl = declaration_statement();
    if (decl == nullptr) { return statements; }
    statements.emplace_back(decl);
  }
  return statements;
}
void parser::synchronize_parser() {
  advance();
  while (!is_at_end()) {
    if (previous()->type_ == token_type::NEW_LINE) { return; }
    switch (peek()->type_) {
      case token_type::KEYWORD_CLASS:
      case token_type::KEYWORD_FOR:
      case token_type::KEYWORD_DEF:
      case token_type::KEYWORD_IF:
      case token_type::KEYWORD_WHILE:
      case token_type::KEYWORD_RETURN:
        return;
      default:
        break; /* do nothing for others */
    }
    advance();
  }
}
stmt *parser::statement() {
  // statement ->  print
  //             | if
  //             | pass
  //             | while
  //             | expression_statement
  if (match({token_type::KEYWORD_PRINT})) { return print_statement(); }
  if (match({token_type::KEYWORD_IF})) { return if_statement(); }
  if (match({token_type::KEYWORD_PASS})) { return pass_statement(); }
  if (match({token_type::KEYWORD_WHILE})) { return while_statement(); }
  return expression_statement();
}
stmt *parser::pass_statement() {
  auto pass_tok = previous();
  consume_or_eof(token_type::NEW_LINE,
                 "Expect new line after value for expression.");
  return pool_.c_pass_stmt(pass_tok);
}
stmt *parser::if_statement() {
  // if_stmt   -> KEYWORD_IF EXPRESSION block_stmt
  //             (KEYWORD_ELSE block_stmt)?
  auto if_keyword = previous();
  auto exp = expression();
  stmt *if_branch = block_statement();
  token *else_keyword = nullptr;
  stmt *else_branch = nullptr;
  if (match({token_type::KEYWORD_ELSE})) {
    else_keyword = previous();
    else_branch = block_statement();
  }
  return pool_.c_if_stmt(if_keyword, exp, if_branch, else_keyword, else_branch);
}
stmt *parser::block_statement() {
  // block_stmt -> COLON NEW_LINE BA_INDENT statement+ BA_DEDENT
  consume(token_type::COLON, "Expected ':' at start of block");
  auto colon = previous();
  consume(token_type::NEW_LINE, "Expected new line");
  consume(token_type::BA_INDENT, "Expected indented block");
  std::vector<stmt *> statements{};
  statements.reserve(3);
  while (!is_at_end()) {
    auto decl = declaration_statement();
    if (decl == nullptr) { break; }
    statements.emplace_back(decl);
    if (peek()->type_ == token_type::BA_DEDENT) { break; }
  }
  if (statements.empty()) { throw error(colon, "Block cannot be empty."); }
  // TODO double check if we really to check of EOF here
  consume(token_type::BA_DEDENT, "Expected dedent");
  return pool_.c_block_stmt(statements);
}
stmt *parser::print_statement() {
  expr *exp = expression();
  consume_or_eof(token_type::NEW_LINE,
                 "Expect new line after value for print statement.");
  return pool_.c_print_stmt(previous(), exp);
}
stmt *parser::expression_statement() {
  expr *exp = expression();
  consume_or_eof(token_type::NEW_LINE,
                 "Expect new line after value for expression.");
  return pool_.c_expression_stmt(exp);
}
token *parser::consume_or_eof(token_type t, const std::string &message) {
  if (check(t)) return advance();
  if (is_at_end()) return peek();
  throw error(peek(), message);
}
stmt *parser::declaration_statement() {
  token *var_name = nullptr;
  expr *exp = nullptr;
  token *data_type = nullptr;
  try {
    if (!match({token_type::NAME})) { return statement(); }
    var_name = previous();
    // Colon should come after name for a variable declaration
    if (!match({token_type::COLON})) {
      recede();
      return statement();
    }
    if (!match({token_type::NAME})) {
      throw error(peek(), "Must have a data type for variable declaration.");
    }
    data_type = previous();
    // `= expression` bit is optional
    if (match({token_type::EQ})) { exp = expression(); }
    consume_or_eof(token_type::NEW_LINE,
                   "Expect new line after value for variable declaration.");
    return pool_.c_let_stmt(var_name, data_type, exp);
  } catch (parsing_error &err) {
    synchronize_parser();
    return nullptr;
  }
}
expr *parser::assignment() {
  auto exp = or_op();
  if (match({token_type::EQ})) {
    auto equals = previous();
    auto val = assignment();
    if (exp->get_type() == ast_type::EXPR_VARIABLE) {
      auto name = (dynamic_cast<variable_expr *>(exp))->name_;
      return pool_.c_assign_expr(name, equals, val);
    }
    throw error(equals, "Invalid assignment target!");
  }
  return exp;
}
expr *parser::or_op() {
  auto exp = and_op();
  while (match({token_type::KEYWORD_OR})) {
    auto opr = previous();
    auto right = and_op();
    exp = pool_.c_logical_expr(exp, opr, right);
  }
  return exp;
}
expr *parser::and_op() {
  auto exp = equality();
  while (match({token_type::KEYWORD_AND})) {
    auto opr = previous();
    auto right = equality();
    exp = pool_.c_logical_expr(exp, opr, right);
  }
  return exp;
}
stmt *parser::while_statement() {
  // while_stmt   -> KEYWORD_WHILE EXPRESSION block_stmt
  auto while_keyword = previous();
  auto exp = expression();
  stmt *while_body = block_statement();
  return pool_.c_while_stmt(while_keyword, exp, while_body);
}
parser::~parser() = default;
