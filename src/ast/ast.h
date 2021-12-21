// ast.h
// generated by update_ast.py
#ifndef AST_H
#define AST_H
#include "tokenizer/token.h"
#include <vector>
namespace yaksha {
// ------ forward declarations ------
struct expr;
struct stmt;
struct assign_expr;
struct binary_expr;
struct grouping_expr;
struct literal_expr;
struct logical_expr;
struct unary_expr;
struct variable_expr;
struct block_stmt;
struct expression_stmt;
struct if_stmt;
struct let_stmt;
struct pass_stmt;
struct print_stmt;
struct return_stmt;
struct while_stmt;
// Types of expressions and statements
enum class ast_type {
  EXPR_ASSIGN,
  EXPR_BINARY,
  EXPR_GROUPING,
  EXPR_LITERAL,
  EXPR_LOGICAL,
  EXPR_UNARY,
  EXPR_VARIABLE,
  STMT_BLOCK,
  STMT_EXPRESSION,
  STMT_IF,
  STMT_LET,
  STMT_PASS,
  STMT_PRINT,
  STMT_RETURN,
  STMT_WHILE
};
// ------ expression visitor ------
struct expr_visitor {
  virtual void visit_assign_expr(assign_expr *obj) = 0;
  virtual void visit_binary_expr(binary_expr *obj) = 0;
  virtual void visit_grouping_expr(grouping_expr *obj) = 0;
  virtual void visit_literal_expr(literal_expr *obj) = 0;
  virtual void visit_logical_expr(logical_expr *obj) = 0;
  virtual void visit_unary_expr(unary_expr *obj) = 0;
  virtual void visit_variable_expr(variable_expr *obj) = 0;
  virtual ~expr_visitor() = default;
};
// ------ statement visitor ------
struct stmt_visitor {
  virtual void visit_block_stmt(block_stmt *obj) = 0;
  virtual void visit_expression_stmt(expression_stmt *obj) = 0;
  virtual void visit_if_stmt(if_stmt *obj) = 0;
  virtual void visit_let_stmt(let_stmt *obj) = 0;
  virtual void visit_pass_stmt(pass_stmt *obj) = 0;
  virtual void visit_print_stmt(print_stmt *obj) = 0;
  virtual void visit_return_stmt(return_stmt *obj) = 0;
  virtual void visit_while_stmt(while_stmt *obj) = 0;
  virtual ~stmt_visitor() = default;
};
// ------ expression base class ------
struct expr {
  virtual ~expr() = default;
  virtual void accept(expr_visitor *v) = 0;
  virtual ast_type get_type() = 0;
};
// ------- statement base class ------
struct stmt {
  virtual ~stmt() = default;
  virtual void accept(stmt_visitor *v) = 0;
  virtual ast_type get_type() = 0;
};
// ------- expressions ------
struct assign_expr : expr {
  assign_expr(token* name, token* opr, expr* right);
  void accept(expr_visitor *v) override;
  ast_type get_type() override;
  token* name_;
  token* opr_;
  expr* right_;
};
struct binary_expr : expr {
  binary_expr(expr* left, token* opr, expr* right);
  void accept(expr_visitor *v) override;
  ast_type get_type() override;
  expr* left_;
  token* opr_;
  expr* right_;
};
struct grouping_expr : expr {
  explicit grouping_expr(expr* expression);
  void accept(expr_visitor *v) override;
  ast_type get_type() override;
  expr* expression_;
};
struct literal_expr : expr {
  explicit literal_expr(token* literal_token);
  void accept(expr_visitor *v) override;
  ast_type get_type() override;
  token* literal_token_;
};
struct logical_expr : expr {
  logical_expr(expr* left, token* opr, expr* right);
  void accept(expr_visitor *v) override;
  ast_type get_type() override;
  expr* left_;
  token* opr_;
  expr* right_;
};
struct unary_expr : expr {
  unary_expr(token* opr, expr* right);
  void accept(expr_visitor *v) override;
  ast_type get_type() override;
  token* opr_;
  expr* right_;
};
struct variable_expr : expr {
  explicit variable_expr(token* name);
  void accept(expr_visitor *v) override;
  ast_type get_type() override;
  token* name_;
};
// ------- statements ------
struct block_stmt : stmt {
  explicit block_stmt(std::vector<stmt*> statements);
  void accept(stmt_visitor *v) override;
  ast_type get_type() override;
  std::vector<stmt*> statements_;
};
struct expression_stmt : stmt {
  explicit expression_stmt(expr* expression);
  void accept(stmt_visitor *v) override;
  ast_type get_type() override;
  expr* expression_;
};
struct if_stmt : stmt {
  if_stmt(token* if_keyword, expr* expression, stmt* if_branch, token* else_keyword, stmt* else_branch);
  void accept(stmt_visitor *v) override;
  ast_type get_type() override;
  token* if_keyword_;
  expr* expression_;
  stmt* if_branch_;
  token* else_keyword_;
  stmt* else_branch_;
};
struct let_stmt : stmt {
  let_stmt(token* name, token* data_type, expr* expression);
  void accept(stmt_visitor *v) override;
  ast_type get_type() override;
  token* name_;
  token* data_type_;
  expr* expression_;
};
struct pass_stmt : stmt {
  explicit pass_stmt(token* pass_token);
  void accept(stmt_visitor *v) override;
  ast_type get_type() override;
  token* pass_token_;
};
struct print_stmt : stmt {
  print_stmt(token* print_keyword, expr* expression);
  void accept(stmt_visitor *v) override;
  ast_type get_type() override;
  token* print_keyword_;
  expr* expression_;
};
struct return_stmt : stmt {
  return_stmt(token* return_keyword, expr* expression);
  void accept(stmt_visitor *v) override;
  ast_type get_type() override;
  token* return_keyword_;
  expr* expression_;
};
struct while_stmt : stmt {
  while_stmt(token* while_keyword, expr* expression, stmt* while_body);
  void accept(stmt_visitor *v) override;
  ast_type get_type() override;
  token* while_keyword_;
  expr* expression_;
  stmt* while_body_;
};
// ------- utils ------
struct ast_pool {
  ast_pool();
  ~ast_pool();
  expr *c_assign_expr(token* name, token* opr, expr* right);
  expr *c_binary_expr(expr* left, token* opr, expr* right);
  expr *c_grouping_expr(expr* expression);
  expr *c_literal_expr(token* literal_token);
  expr *c_logical_expr(expr* left, token* opr, expr* right);
  expr *c_unary_expr(token* opr, expr* right);
  expr *c_variable_expr(token* name);
  stmt *c_block_stmt(std::vector<stmt*> statements);
  stmt *c_expression_stmt(expr* expression);
  stmt *c_if_stmt(token* if_keyword, expr* expression, stmt* if_branch, token* else_keyword, stmt* else_branch);
  stmt *c_let_stmt(token* name, token* data_type, expr* expression);
  stmt *c_pass_stmt(token* pass_token);
  stmt *c_print_stmt(token* print_keyword, expr* expression);
  stmt *c_return_stmt(token* return_keyword, expr* expression);
  stmt *c_while_stmt(token* while_keyword, expr* expression, stmt* while_body);

private:
  std::vector<expr *> cleanup_expr_;
  std::vector<stmt *> cleanup_stmt_;
};
} // namespace yaksha
#endif