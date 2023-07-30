// type_checker.h
#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H
#include "ast/ast.h"
#include "ast/codefiles.h"
#include "ast/environment_stack.h"
#include "builtins/builtins.h"
#include "compiler/def_class_visitor.h"
#include "compiler/slot_matcher.h"
#include "utilities/ykobject.h"
namespace yaksha {
  struct type_checker : expr_visitor, stmt_visitor, slot_matcher {
    explicit type_checker(std::string filepath, codefiles *cf,
                          def_class_visitor *dcv, ykdt_pool *pool,
                          gc_pool<token> *token_pool);
    ~type_checker() override;
    bool slot_match(const ykobject &arg, ykdatatype *datatype) override;
    ykdatatype *function_to_datatype(const ykobject &arg) override;
    void check(const std::vector<stmt *> &statements);
    void visit_assign_expr(assign_expr *obj) override;
    void visit_binary_expr(binary_expr *obj) override;
    void visit_fncall_expr(fncall_expr *obj) override;
    void visit_grouping_expr(grouping_expr *obj) override;
    void visit_literal_expr(literal_expr *obj) override;
    void visit_logical_expr(logical_expr *obj) override;
    void visit_unary_expr(unary_expr *obj) override;
    void visit_variable_expr(variable_expr *obj) override;
    void visit_block_stmt(block_stmt *obj) override;
    void visit_break_stmt(break_stmt *obj) override;
    void visit_continue_stmt(continue_stmt *obj) override;
    void visit_def_stmt(def_stmt *obj) override;
    void visit_class_stmt(class_stmt *obj) override;
    void visit_expression_stmt(expression_stmt *obj) override;
    void visit_if_stmt(if_stmt *obj) override;
    void visit_let_stmt(let_stmt *obj) override;
    void visit_pass_stmt(pass_stmt *obj) override;
    void visit_return_stmt(return_stmt *obj) override;
    void visit_while_stmt(while_stmt *obj) override;
    void visit_defer_stmt(defer_stmt *obj) override;
    void visit_del_stmt(del_stmt *obj) override;
    void visit_get_expr(get_expr *obj) override;
    void visit_set_expr(set_expr *obj) override;
    void visit_assign_member_expr(assign_member_expr *obj) override;
    void
    visit_square_bracket_access_expr(square_bracket_access_expr *obj) override;
    void visit_assign_arr_expr(assign_arr_expr *obj) override;
    void visit_square_bracket_set_expr(square_bracket_set_expr *obj) override;
    void visit_ccode_stmt(ccode_stmt *obj) override;
    void visit_import_stmt(import_stmt *obj) override;
    void visit_const_stmt(const_stmt *obj) override;
    void visit_runtimefeature_stmt(runtimefeature_stmt *obj) override;
    void visit_nativeconst_stmt(nativeconst_stmt *obj) override;
    void visit_foreach_stmt(foreach_stmt *obj) override;
    void visit_forendless_stmt(forendless_stmt *obj) override;
    void visit_compins_stmt(compins_stmt *obj) override;
    void visit_curly_call_expr(curly_call_expr *obj) override;
    void visit_macro_call_expr(macro_call_expr *obj) override;
    /**
     * Errors vector, type checker will try and identify as much errors as possible
     * but after first error, everything else is best guess
     */
    std::vector<parsing_error> errors_;
    /**
     * Get function information
     */
    def_class_visitor *defs_classes_;
    // Scope
    environment_stack scope_;
    codefiles *cf_;
    std::string filepath_;

private:
    ykobject pop();
    class_stmt *find_class(token *tok, ykdatatype *data_type);
    void push(const ykobject &data_type);
    void error(token *tok, const std::string &message);
    void error(const std::string &message);
    void push_scope_type(ast_type scope_type);
    ast_type peek_scope_type();
    void pop_scope_type();
    void push_function(const std::string &prefixed_name);
    std::string peek_function();
    void pop_function();
    void handle_dot_operator(expr *lhs_expr, token *dot, token *member_item);
    void handle_square_access(expr *index_expr, token *sqb_token,
                              expr *name_expr);
    void handle_assigns(token *oper, const ykobject &lhs, const ykobject &rhs);
    // Data type pool
    ykdt_pool *dt_pool_;
    std::vector<ykobject> object_stack_{};
    // Different types of scopes stack, -> are we in function body, if or while
    std::vector<ast_type> scope_type_stack_{};
    // Function stack
    std::vector<std::string> function_name_stack_{};
    // Builtin functions
    builtins builtins_;
    // Copy of internal stmt_alias to handle dt parsing in builtins
    std::unordered_map<std::string, import_stmt *> import_stmts_alias_{};
    void validate_member(name_val &member, class_stmt *class_st);
  };
}// namespace yaksha
#endif
