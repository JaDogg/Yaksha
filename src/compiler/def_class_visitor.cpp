// def_class_visitor.cpp
#include "def_class_visitor.h"
#include "builtins.h"
#include "compiler_utils.h"
using namespace yaksha;
def_class_visitor::def_class_visitor() = default;
def_class_visitor::~def_class_visitor() = default;
void def_class_visitor::visit_assign_expr(assign_expr *obj) {}
void def_class_visitor::visit_binary_expr(binary_expr *obj) {}
void def_class_visitor::visit_fncall_expr(fncall_expr *obj) {}
void def_class_visitor::visit_grouping_expr(grouping_expr *obj) {}
void def_class_visitor::visit_literal_expr(literal_expr *obj) {}
void def_class_visitor::visit_logical_expr(logical_expr *obj) {}
void def_class_visitor::visit_unary_expr(unary_expr *obj) {}
void def_class_visitor::visit_variable_expr(variable_expr *obj) {}
void def_class_visitor::visit_block_stmt(block_stmt *obj) {}
void def_class_visitor::visit_break_stmt(break_stmt *obj) {}
void def_class_visitor::visit_continue_stmt(continue_stmt *obj) {}
void def_class_visitor::visit_def_stmt(def_stmt *obj) {
  auto name = obj->name_->token_;
  if (builtins::has_builtin(name)) {
    error(obj->name_, "Critical!! Redefinition of builtin function");
    return;
  }
  if (has_function(name)) {
    error(obj->name_, "Critical!! Redefinition of function");
    return;
  }
  if (has_class(name)) {
    error(obj->name_, "Critical!! Redefinition of class as a function");
    return;
  }
  if (obj->annotations_.varargs_ &&
      (!obj->annotations_.native_macro_ && !obj->annotations_.native_define_)) {
    error(obj->name_,
          "@vararg must be used with @nativemacro or @nativedefine.");
    return;
  }
  if (obj->annotations_.varargs_ && obj->params_.size() <= 1) {
    error(obj->name_,
          "@varargs only works with functions that have at least 2 arguments.");
    return;
  }
  function_names_.push_back(name);
  functions_.insert({name, obj});
}
void def_class_visitor::visit_expression_stmt(expression_stmt *obj) {}
void def_class_visitor::visit_if_stmt(if_stmt *obj) {}
void def_class_visitor::visit_let_stmt(let_stmt *obj) {}
void def_class_visitor::visit_pass_stmt(pass_stmt *obj) {}
void def_class_visitor::visit_return_stmt(return_stmt *obj) {}
void def_class_visitor::visit_while_stmt(while_stmt *obj) {}
void def_class_visitor::extract(const std::vector<stmt *> &statements) {
  for (auto st : statements) { st->accept(this); }
}
void def_class_visitor::visit_defer_stmt(defer_stmt *obj) {}
void def_class_visitor::error(token *tok, const std::string &message) {
  auto err = parsing_error{message, tok};
  errors_.emplace_back(err);
}
def_stmt *def_class_visitor::get_function(const std::string &prefixed_name) {
  if (has_function(prefixed_name)) { return functions_[prefixed_name]; }
  return nullptr;
}
bool def_class_visitor::has_function(const std::string &prefixed_name) {
  return functions_.find(prefixed_name) != functions_.end();
}
void def_class_visitor::visit_class_stmt(class_stmt *obj) {
  auto name = obj->name_->token_;
  if (obj->annotations_.varargs_ || obj->annotations_.native_ ||
      obj->annotations_.template_) {
    error(obj->name_,
          "@varargs, @native and @template are not allowed for classes.");
    return;
  }
  if (obj->annotations_.native_define_ &&
      obj->annotations_.native_define_arg_.empty()) {
    error(obj->name_, "@nativedefine must have a valid argument");
    return;
  }
  if (builtins::has_builtin(name)) {
    error(obj->name_, "Critical!! Redefinition of builtin function");
    return;
  }
  if (has_class(name)) {
    error(obj->name_, "Critical!! Redefinition of class");
    return;
  }
  if (has_function(name)) {
    error(obj->name_, "Critical!! Redefinition of function as a class");
    return;
  }
  class_names_.push_back(name);
  classes_.insert({name, obj});
}
class_stmt *def_class_visitor::get_class(const std::string &prefixed_name) {
  if (has_class(prefixed_name)) { return classes_[prefixed_name]; }
  return nullptr;
}
bool def_class_visitor::has_class(const std::string &prefixed_name) {
  return classes_.find(prefixed_name) != classes_.end();
}
void def_class_visitor::visit_del_stmt(del_stmt *obj) {}
void def_class_visitor::visit_get_expr(get_expr *obj) {}
void def_class_visitor::visit_set_expr(set_expr *obj) {}
void def_class_visitor::visit_assign_member_expr(assign_member_expr *obj) {}
void def_class_visitor::visit_square_bracket_access_expr(
    square_bracket_access_expr *obj) {}
void def_class_visitor::visit_assign_arr_expr(assign_arr_expr *obj) {}
void def_class_visitor::visit_square_bracket_set_expr(
    square_bracket_set_expr *obj) {}
void def_class_visitor::visit_ccode_stmt(ccode_stmt *obj) {}
void def_class_visitor::visit_import_stmt(import_stmt *obj) {}
