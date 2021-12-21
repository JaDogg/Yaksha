// ast.cpp
// generated by update_ast.py
#include "ast/ast.h"
using namespace yaksha;
// ------- expressions -----
assign_expr::assign_expr(token* name, token* opr, expr* right)
    : name_(name), opr_(opr), right_(right) {}
void assign_expr::accept(expr_visitor *v) {
  v->visit_assign_expr(this);
}
ast_type assign_expr::get_type() {
  return ast_type::EXPR_ASSIGN;
}
expr *ast_pool::c_assign_expr(token* name, token* opr, expr* right) {
  auto o = new assign_expr(name, opr, right);
  cleanup_expr_.push_back(o);
  return o;
}
binary_expr::binary_expr(expr* left, token* opr, expr* right)
    : left_(left), opr_(opr), right_(right) {}
void binary_expr::accept(expr_visitor *v) {
  v->visit_binary_expr(this);
}
ast_type binary_expr::get_type() {
  return ast_type::EXPR_BINARY;
}
expr *ast_pool::c_binary_expr(expr* left, token* opr, expr* right) {
  auto o = new binary_expr(left, opr, right);
  cleanup_expr_.push_back(o);
  return o;
}
grouping_expr::grouping_expr(expr* expression)
    : expression_(expression) {}
void grouping_expr::accept(expr_visitor *v) {
  v->visit_grouping_expr(this);
}
ast_type grouping_expr::get_type() {
  return ast_type::EXPR_GROUPING;
}
expr *ast_pool::c_grouping_expr(expr* expression) {
  auto o = new grouping_expr(expression);
  cleanup_expr_.push_back(o);
  return o;
}
literal_expr::literal_expr(token* literal_token)
    : literal_token_(literal_token) {}
void literal_expr::accept(expr_visitor *v) {
  v->visit_literal_expr(this);
}
ast_type literal_expr::get_type() {
  return ast_type::EXPR_LITERAL;
}
expr *ast_pool::c_literal_expr(token* literal_token) {
  auto o = new literal_expr(literal_token);
  cleanup_expr_.push_back(o);
  return o;
}
logical_expr::logical_expr(expr* left, token* opr, expr* right)
    : left_(left), opr_(opr), right_(right) {}
void logical_expr::accept(expr_visitor *v) {
  v->visit_logical_expr(this);
}
ast_type logical_expr::get_type() {
  return ast_type::EXPR_LOGICAL;
}
expr *ast_pool::c_logical_expr(expr* left, token* opr, expr* right) {
  auto o = new logical_expr(left, opr, right);
  cleanup_expr_.push_back(o);
  return o;
}
unary_expr::unary_expr(token* opr, expr* right)
    : opr_(opr), right_(right) {}
void unary_expr::accept(expr_visitor *v) {
  v->visit_unary_expr(this);
}
ast_type unary_expr::get_type() {
  return ast_type::EXPR_UNARY;
}
expr *ast_pool::c_unary_expr(token* opr, expr* right) {
  auto o = new unary_expr(opr, right);
  cleanup_expr_.push_back(o);
  return o;
}
variable_expr::variable_expr(token* name)
    : name_(name) {}
void variable_expr::accept(expr_visitor *v) {
  v->visit_variable_expr(this);
}
ast_type variable_expr::get_type() {
  return ast_type::EXPR_VARIABLE;
}
expr *ast_pool::c_variable_expr(token* name) {
  auto o = new variable_expr(name);
  cleanup_expr_.push_back(o);
  return o;
}
// ------- statements -----
block_stmt::block_stmt(std::vector<stmt*> statements)
    : statements_(statements) {}
void block_stmt::accept(stmt_visitor *v) {
  v->visit_block_stmt(this);
}
ast_type block_stmt::get_type() {
  return ast_type::STMT_BLOCK;
}
stmt *ast_pool::c_block_stmt(std::vector<stmt*> statements) {
  auto o = new block_stmt(statements);
  cleanup_stmt_.push_back(o);
  return o;
}
expression_stmt::expression_stmt(expr* expression)
    : expression_(expression) {}
void expression_stmt::accept(stmt_visitor *v) {
  v->visit_expression_stmt(this);
}
ast_type expression_stmt::get_type() {
  return ast_type::STMT_EXPRESSION;
}
stmt *ast_pool::c_expression_stmt(expr* expression) {
  auto o = new expression_stmt(expression);
  cleanup_stmt_.push_back(o);
  return o;
}
if_stmt::if_stmt(token* if_keyword, expr* expression, stmt* if_branch, token* else_keyword, stmt* else_branch)
    : if_keyword_(if_keyword), expression_(expression), if_branch_(if_branch), else_keyword_(else_keyword), else_branch_(else_branch) {}
void if_stmt::accept(stmt_visitor *v) {
  v->visit_if_stmt(this);
}
ast_type if_stmt::get_type() {
  return ast_type::STMT_IF;
}
stmt *ast_pool::c_if_stmt(token* if_keyword, expr* expression, stmt* if_branch, token* else_keyword, stmt* else_branch) {
  auto o = new if_stmt(if_keyword, expression, if_branch, else_keyword, else_branch);
  cleanup_stmt_.push_back(o);
  return o;
}
let_stmt::let_stmt(token* name, token* data_type, expr* expression)
    : name_(name), data_type_(data_type), expression_(expression) {}
void let_stmt::accept(stmt_visitor *v) {
  v->visit_let_stmt(this);
}
ast_type let_stmt::get_type() {
  return ast_type::STMT_LET;
}
stmt *ast_pool::c_let_stmt(token* name, token* data_type, expr* expression) {
  auto o = new let_stmt(name, data_type, expression);
  cleanup_stmt_.push_back(o);
  return o;
}
pass_stmt::pass_stmt(token* pass_token)
    : pass_token_(pass_token) {}
void pass_stmt::accept(stmt_visitor *v) {
  v->visit_pass_stmt(this);
}
ast_type pass_stmt::get_type() {
  return ast_type::STMT_PASS;
}
stmt *ast_pool::c_pass_stmt(token* pass_token) {
  auto o = new pass_stmt(pass_token);
  cleanup_stmt_.push_back(o);
  return o;
}
print_stmt::print_stmt(token* print_keyword, expr* expression)
    : print_keyword_(print_keyword), expression_(expression) {}
void print_stmt::accept(stmt_visitor *v) {
  v->visit_print_stmt(this);
}
ast_type print_stmt::get_type() {
  return ast_type::STMT_PRINT;
}
stmt *ast_pool::c_print_stmt(token* print_keyword, expr* expression) {
  auto o = new print_stmt(print_keyword, expression);
  cleanup_stmt_.push_back(o);
  return o;
}
return_stmt::return_stmt(token* return_keyword, expr* expression)
    : return_keyword_(return_keyword), expression_(expression) {}
void return_stmt::accept(stmt_visitor *v) {
  v->visit_return_stmt(this);
}
ast_type return_stmt::get_type() {
  return ast_type::STMT_RETURN;
}
stmt *ast_pool::c_return_stmt(token* return_keyword, expr* expression) {
  auto o = new return_stmt(return_keyword, expression);
  cleanup_stmt_.push_back(o);
  return o;
}
while_stmt::while_stmt(token* while_keyword, expr* expression, stmt* while_body)
    : while_keyword_(while_keyword), expression_(expression), while_body_(while_body) {}
void while_stmt::accept(stmt_visitor *v) {
  v->visit_while_stmt(this);
}
ast_type while_stmt::get_type() {
  return ast_type::STMT_WHILE;
}
stmt *ast_pool::c_while_stmt(token* while_keyword, expr* expression, stmt* while_body) {
  auto o = new while_stmt(while_keyword, expression, while_body);
  cleanup_stmt_.push_back(o);
  return o;
}
// ------- utils ------
ast_pool::~ast_pool() {
  for (auto e : cleanup_expr_) {
    delete e;
  }
  for (auto e : cleanup_stmt_) {
    delete e;
  }
  cleanup_expr_.clear();
  cleanup_stmt_.clear();
}
ast_pool::ast_pool() {
  cleanup_expr_.reserve(1000);
  cleanup_stmt_.reserve(1000);
}