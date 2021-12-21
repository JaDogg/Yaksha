// ast_vis.h
#ifndef AST_VIS_H
#define AST_VIS_H
#include "ast/ast.h"
#include <sstream>
#include <string>
namespace yaksha {
  /**
   * Print Abstract Syntax Tree as HTML
   */
  struct ast_vis : stmt_visitor, expr_visitor {
    explicit ast_vis();
    ~ast_vis() override = default;
    void visit_binary_expr(binary_expr *obj) override;
    void visit_grouping_expr(grouping_expr *obj) override;
    void visit_literal_expr(literal_expr *obj) override;
    void visit_unary_expr(unary_expr *obj) override;
    void visit_return_stmt(return_stmt *obj) override;
    void field(const std::string &name, expr *expr);
    void field(const std::string &name, stmt *stmt);
    void field(const std::string &name, const std::string &literal_obj);
    void begin_block(const std::string &name);
    void end_block();
    /**
     * Print given expression as HTML
     * @param st expression
     */
    void print(expr *st);
    /**
     * Print given vector of statement as HTML
     * @param statements vector of statements
     */
    void print(const std::vector<stmt *> &statements);
    void visit_expression_stmt(expression_stmt *obj) override;
    void visit_print_stmt(print_stmt *obj) override;
    void visit_let_stmt(let_stmt *obj) override;
    void visit_block_stmt(block_stmt *obj) override;
    void visit_if_stmt(if_stmt *obj) override;
    void visit_pass_stmt(pass_stmt *obj) override;
    void visit_variable_expr(variable_expr *obj) override;
    void visit_assign_expr(assign_expr *obj) override;
    void visit_logical_expr(logical_expr *obj) override;
    void visit_while_stmt(while_stmt *obj) override;
    void visit_break_stmt(break_stmt *obj) override;
    void visit_continue_stmt(continue_stmt *obj) override;

private:
    std::stringstream text_{};
  };
}// namespace yaksha
#endif
