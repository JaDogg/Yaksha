// ==============================================================================================
// ╦  ┬┌─┐┌─┐┌┐┌┌─┐┌─┐    Yaksha Programming Language
// ║  ││  ├┤ │││└─┐├┤     is Licensed with GPLv3 + extra terms. Please see below.
// ╩═╝┴└─┘└─┘┘└┘└─┘└─┘
// Note: libs - MIT license, runtime/3rd - various
// ==============================================================================================
// GPLv3:
//
// Yaksha - Programming Language.
// Copyright (C) 2020 - 2024 Bhathiya Perera
//
// This program is free software: you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program.
// If not, see https://www.gnu.org/licenses/.
//
// ==============================================================================================
// Additional Terms:
//
// Please note that any commercial use of the programming language's compiler source code
// (everything except compiler/runtime, compiler/libs and compiler/3rd) require a written agreement
// with author of the language (Bhathiya Perera).
//
// If you are using it for an open source project, please give credits.
// Your own project must use GPLv3 license with these additional terms.
//
// You may use programs written in Yaksha/YakshaLisp for any legal purpose
// (commercial, open-source, closed-source, etc) as long as it agrees
// to the licenses of linked runtime libraries (see compiler/runtime/README.md).
//
// ==============================================================================================
// builtins.cpp
#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
#include "builtins.h"
#include "ast/parser.h"
#include "tokenizer/tokenizer.h"
#include <sstream>
#include <utility>
using namespace yaksha;
// http://patorjk.com/software/taag/#p=display&f=Calvin%20S&t=Type%20Something%20
//
// ┌─┐┬─┐┬─┐┌─┐┬ ┬┌┬┐
// ├─┤├┬┘├┬┘├─┘│ │ │
// ┴ ┴┴└─┴└─┴  └─┘ ┴
//
struct builtin_arrput : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for arrput() builtin";
    } else if (!args[0].datatype_->is_array()) {
      o.string_val_ = "First argument to arrput() must be an Array[?]";
    } else if (args[0].datatype_->args_[0]->is_m_entry() ||
               args[0].datatype_->args_[0]->is_sm_entry()) {
      o.string_val_ = "arrput() does not work with maps";
    } else {
      auto matched = dt_slot_matcher->type_match(
          args[0].datatype_->args_[0], args[1].datatype_->const_unwrap(),
          args[1].is_primitive_or_obj());
      if (matched.matched_) { return o; }
      o.string_val_ = "Argument to arrput() does not match the array type " +
                      matched.error_;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    if (args[1].second.datatype_->const_unwrap()->is_a_string()) {
      code << "yk__arrput(" << args[0].first << ", ";
      dt_compiler->compile_string_assign(
          nullptr, code, args[1], args[1].second.datatype_->const_unwrap(),
          args[0].second.datatype_->args_[0]->const_unwrap());
      code << ")";
    } else {
      code << "yk__arrput(" << args[0].first << ", " << args[1].first << ")";
    }
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
struct builtin_arrins : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 3) {
      o.string_val_ = "Three arguments must be provided for arrins() builtin";
    } else if (!args[0].datatype_->is_array() ||
               args[0].datatype_->args_[0]->is_sm_entry()) {
      o.string_val_ =
          "First argument to arrins() must be an Array[?]";
    } else if (!args[1].datatype_->const_unwrap()->is_an_integer()) {
      o.string_val_ = "Second argument to arrins() must be an integer";
    } else {
      // Given arrins(Array[T], int, V)
      // ---
      // LHS -> Array[T] -> T
      auto lhs = args[0].datatype_->args_[0]->const_unwrap();
      // RHS -> V
      auto rhs = args[2].datatype_->const_unwrap();
      // T == V
      auto matched =
          dt_slot_matcher->type_match(lhs, rhs, args[2].is_primitive_or_obj());
      if (matched.matched_) {
        return o;
      }
      o.string_val_ =
          "Third argument to arrins() must match provided array " + matched.error_;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "yk__arrins(" << args[0].first << ", ";
    dt_compiler->compile_string_assign(nullptr, code, args[1],
                                       args[1].second.datatype_->const_unwrap(),
                                       dt_pool->create("str"));
    code << ", " << args[2].first << ")";
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┌─┐┬─┐┬─┐┌─┐┌─┐┌─┐
// ├─┤├┬┘├┬┘├─┘│ │├─┘
// ┴ ┴┴└─┴└─┴  └─┘┴
//
struct builtin_arrpop : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 1) {
      o.string_val_ = "One argument must be provided for arrpop() builtin";
    } else if (!args[0].datatype_->is_array()) {
      o.string_val_ = "Argument to arrpop() must be an Array[?]";
    } else if (args[0].datatype_->args_[0]->is_m_entry() ||
               args[0].datatype_->args_[0]->is_sm_entry()) {
      o.string_val_ = "arrpop() does not work with maps";
    } else {
      return yk_object(args[0].datatype_->args_[0]);
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    o = yk_object(args[0].second.datatype_->args_[0]);
    code << "yk__arrpop(" << args[0].first << ")";
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┌─┐┬─┐┬─┐┌─┐┌─┐┌┬┐┌─┐┌─┐┌─┐
// ├─┤├┬┘├┬┘└─┐├┤  │ │  ├─┤├─┘
// ┴ ┴┴└─┴└─└─┘└─┘ ┴ └─┘┴ ┴┴
//
// ┌─┐┬─┐┬─┐┌─┐┌─┐┌┬┐┬  ┌─┐┌┐┌
// ├─┤├┬┘├┬┘└─┐├┤  │ │  ├┤ │││
// ┴ ┴┴└─┴└─└─┘└─┘ ┴ ┴─┘└─┘┘└┘
//
// ┌─┐┬─┐┬─┐┌┬┐┌─┐┬
// ├─┤├┬┘├┬┘ ││├┤ │
// ┴ ┴┴└─┴└──┴┘└─┘┴─┘
struct builtin_arrsetlencap : builtin {
  explicit builtin_arrsetlencap(std::string function_name)
      : func_name_(std::move(function_name)) {}
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ =
          "Two arguments must be provided for " + func_name_ + "() builtin";
    } else if (!args[0].datatype_->is_array()) {
      o.string_val_ =
          "First argument to " + func_name_ + "() must be an Array[?]";
    } else if (args[0].datatype_->args_[0]->is_m_entry() ||
               args[0].datatype_->args_[0]->is_sm_entry()) {
      o.string_val_ = func_name_ + "() does not work with maps";
    } else if (!(args[1].datatype_->const_unwrap()->is_an_integer())) {
      o.string_val_ = func_name_ + "Second argument to () must be an integer";
    } else {
      return yk_object(dt_pool);// None return
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "yk__" + func_name_ + "(" << args[0].first << ", " << args[1].first
         << ")";
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }

  private:
  std::string func_name_;
};
//
// ┌─┐┬─┐┬┌┐┌┌┬┐
// ├─┘├┬┘││││ │
// ┴  ┴└─┴┘└┘ ┴
// ┌─┐┬─┐┬┌┐┌┌┬┐┬  ┌┐┌
// ├─┘├┬┘││││ │ │  │││
// ┴  ┴└─┴┘└┘ ┴ ┴─┘┘└┘
//
struct builtin_print : builtin {
  explicit builtin_print(std::string func_name)
      : func_name_(std::move(func_name)) {}
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 1) {
      o.string_val_ = "One argument must be provided for print() builtin";
    } else if (!(args[0].datatype_->const_unwrap()->is_primitive())) {
      o.string_val_ = "Argument to print() must be a primitive";
    } else {
      return o;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    auto rhs = args[0];
    yk_datatype *dt = rhs.second.datatype_->const_unwrap();
    if (dt->is_a_signed_integer()) {
      code << "yk__" << func_name_ << "int((intmax_t)" << rhs.first << ")";
    } else if (dt->is_an_unsigned_integer()) {
      code << "yk__" << func_name_ << "uint((uintmax_t)" << rhs.first << ")";
    } else if (dt->is_str()) {
      code << "yk__" << func_name_ << "str(" << rhs.first << ")";
    } else if (dt->is_string_literal()) {
      code << "yk__" << func_name_ << "str(\""
           << string_utils::escape(
                  string_utils::unescape(rhs.second.string_val_))
           << "\")";
    } else if (dt->is_sr()) {
      code << "yk__" << func_name_ << "str(" << "yk__bstr_get_reference("
           << rhs.first << ")" << ")";
    } else if (dt->is_a_float()) {
      code << "yk__" << func_name_ << "dbl(" << rhs.first << ")";
    } else if (dt->is_none()) {
      code << "yk__" << func_name_ << "str(\"None\")";
    } else if (dt->is_bool()) {
      // If bool is a literal just print a string literal
      //   else use a conditional
      if (rhs.first == "true") {
        code << "yk__" << func_name_ << "str(\"True\")";
      } else if (rhs.first == "false") {
        code << "yk__" << func_name_ << "str(\"False\")";
      } else {
        code << "yk__" << func_name_ << "str("
             << dt_compiler->wrap_in_paren(rhs.first)
             << R"( ? "True" : "False"))";
      }
    }
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }

  private:
  std::string func_name_;
};
//
// ┬  ┌─┐┌┐┌
// │  ├┤ │││
// ┴─┘└─┘┘└┘
//
struct builtin_len : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 1) {
      o.string_val_ = "One argument must be provided for len() builtin";
    } else if (!args[0].datatype_->const_unwrap()->is_array() &&
               !args[0].datatype_->const_unwrap()->is_a_string() &&
               !args[0].datatype_->const_unwrap()->is_fixed_size_array()) {
      o.string_val_ = "Argument to len() must be an Array or a string";
    } else {
      return yk_object(dt_pool->create("int"));
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    if (args[0].second.datatype_->const_unwrap()->is_str()) {
      code << "yk__sdslen(" << args[0].first << ")";
    } else if (args[0].second.datatype_->const_unwrap()->is_string_literal()) {
      auto u = string_utils::unescape(args[0].second.string_val_);
      code << u.size();
    } else if (args[0].second.datatype_->const_unwrap()->is_sr()) {
      code << "yk__bstr_len(" << args[0].first << ")";
    } else if (args[0].second.datatype_->const_unwrap()->is_array() &&
               args[0]
                   .second.datatype_->const_unwrap()
                   ->args_[0]
                   ->is_sm_entry()) {
      // Array[SMEntry[V]]
      code << "yk__shlen(" << args[0].first << ")";
    } else if (args[0].second.datatype_->const_unwrap()->is_array() &&
               args[0]
                   .second.datatype_->const_unwrap()
                   ->args_[0]
                   ->is_m_entry()) {
      // Array[MEntry[K,V]]
      code << "yk__hmlen(" << args[0].first << ")";
    } else if (args[0]
                   .second.datatype_->const_unwrap()
                   ->is_fixed_size_array()) {
      code << args[0].second.datatype_->const_unwrap()->dimension_;
    } else {
      code << "yk__arrlen(" << args[0].first << ")";
    }
    o = yk_object(dt_pool->create("int"));
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┌─┐┬ ┬┌─┐┬─┐┌─┐┌┬┐
// │  ├─┤├─┤├┬┘├─┤ │
// └─┘┴ ┴┴ ┴┴└─┴ ┴ ┴
//
struct builtin_charat : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for charat() builtin";
    } else if (!args[0].datatype_->const_unwrap()->is_a_string()) {
      o.string_val_ = "Fist argument to charat() must be a string";
    } else if (!(args[1].datatype_->const_unwrap()->is_an_integer())) {
      o.string_val_ = "Second argument to charat() must be an integer";
    } else {
      return yk_object(dt_pool->create("int"));
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    if (args[0].second.datatype_->const_unwrap()->is_string_literal() ||
        args[0].second.datatype_->const_unwrap()->is_str()) {
      code << "(" << args[0].first << "[" << args[1].first << "])";
    } else {// sr
      code << "(yk__bstr_get_reference(" << args[0].first << ")["
           << args[1].first << "])";
    }
    o = yk_object(dt_pool->create("int"));
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┌─┐┌─┐┌┬┐┬─┐┌─┐┌─┐
// │ ┬├┤  │ ├┬┘├┤ ├┤
// └─┘└─┘ ┴ ┴└─└─┘└
//
struct builtin_getref : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 1) {
      o.string_val_ = "One argument must be provided for getref() builtin";
    } else if (!args[0].is_primitive_or_obj()) {
      o.string_val_ = "Argument to getref() must be an object or primitive";
    } else {
      yk_datatype *dt = dt_pool->create("Ptr");
      dt->args_.emplace_back(args[0].datatype_);
      return yk_object(dt);
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "(&(" << args[0].first << "))";
    yk_datatype *dt = dt_pool->create("Ptr");
    dt->args_.emplace_back(args[0].second.datatype_);
    o = yk_object(dt);
    return {code.str(), o};
  }
  bool require_stdlib() override { return false; }
};
//
// ┬ ┬┌┐┌┬─┐┌─┐┌─┐
// │ ││││├┬┘├┤ ├┤
// └─┘┘└┘┴└─└─┘└
//
struct builtin_unref : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 1) {
      o.string_val_ = "One argument must be provided for unref() builtin";
    } else if (!args[0].datatype_->is_ptr()) {
      o.string_val_ = "Argument to unref() must be a Ptr[?]";
    } else {
      return yk_object(args[0].datatype_->args_[0]);
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "(*(" << args[0].first << "))";
    o = yk_object(args[0].second.datatype_->args_[0]);
    return {code.str(), o};
  }
  bool require_stdlib() override { return false; }
};
// ┌─┐┌─┐┌┬┐┬─┐┌─┐┌─┐
// └─┐├┤  │ ├┬┘├┤ ├┤
// └─┘└─┘ ┴ ┴└─└─┘└
struct builtin_setref : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for setref() builtin";
    } else if (!args[0].datatype_->is_ptr()) {
      o.string_val_ = "First argument to setref() must be a Ptr[?]";
    } else {
      auto matched = dt_slot_matcher->type_match(
          args[0].datatype_->args_[0], args[1].datatype_->const_unwrap(),
          args[1].is_primitive_or_obj());
      if (matched.matched_) { return o; }
      o.string_val_ = "Argument to setref() does not match the pointer type " +
                      matched.error_;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "*(" << args[0].first << ") = " << args[1].first;
    return {code.str(), o};
  }
  bool require_stdlib() override { return false; }
};
// ┌─┐┬ ┬┌┐┌┌─┐┬ ┬
// └─┐├─┤│││├┤ │││
// └─┘┴ ┴┘└┘└─┘└┴┘
//
struct builtin_shnew : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 1) {
      o.string_val_ = "One argument must be provided for shnew() builtin";
    } else if (!args[0].datatype_->is_array() ||
               !args[0].datatype_->args_[0]->is_sm_entry()) {
      o.string_val_ = "Argument to shnew() must match with Array[SMEntry[?]]";
    } else {
      return o;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "yk__sh_new_strdup(" << args[0].first << ")";
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┌─┐┬ ┬┌─┐┌─┐┌┬┐
// └─┐├─┤│ ┬├┤  │
// └─┘┴ ┴└─┘└─┘ ┴
//
struct builtin_shget : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for shget() builtin";
    } else if (!args[0].datatype_->is_array() ||
               !args[0].datatype_->args_[0]->is_sm_entry()) {
      o.string_val_ =
          "First argument to shget() must match with Array[SMEntry[?]]";
    } else if (!args[1].datatype_->const_unwrap()->is_a_string()) {
      o.string_val_ = "Second argument to shget() must be a string";
    } else {
      return yk_object(args[0].datatype_->args_[0]->args_[0]);
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "yk__shget(" << args[0].first << ", ";
    dt_compiler->compile_string_assign(nullptr, code, args[1],
                                       args[1].second.datatype_->const_unwrap(),
                                       dt_pool->create("str"));
    code << ")";
    o = yk_object(args[0].second.datatype_->args_[0]->args_[0]);
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
struct builtin_shdel : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for shdel() builtin";
    } else if (!args[0].datatype_->is_array() ||
               !args[0].datatype_->args_[0]->is_sm_entry()) {
      o.string_val_ =
          "First argument to shdel() must match with Array[SMEntry[?]]";
    } else if (!args[1].datatype_->const_unwrap()->is_a_string()) {
      o.string_val_ = "Second argument to shdel() must be a string";
    } else {
      // All good
      return yk_object(dt_pool->create("bool"));
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "(yk__shdel(" << args[0].first << ", ";
    dt_compiler->compile_string_assign(nullptr, code, args[1],
                                       args[1].second.datatype_->const_unwrap(),
                                       dt_pool->create("str"));
    code << ") == 1)";
    o = yk_object(dt_pool->create("bool"));
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┌─┐┬ ┬┌─┐┌─┐┌┬┐┬
// └─┐├─┤│ ┬├┤  │ │
// └─┘┴ ┴└─┘└─┘ ┴ ┴
//
struct builtin_shgeti : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for shgeti() builtin";
    } else if (!args[0].datatype_->is_array() ||
               !args[0].datatype_->args_[0]->is_sm_entry()) {
      o.string_val_ =
          "First argument to shgeti() must match with Array[SMEntry[?]]";
    } else if (!args[1].datatype_->const_unwrap()->is_a_string()) {
      o.string_val_ = "Second argument to shgeti() must be a string";
    } else {
      return yk_object(dt_pool->create("int"));
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "yk__shgeti(" << args[0].first << ", ";
    dt_compiler->compile_string_assign(nullptr, code, args[1],
                                       args[1].second.datatype_->const_unwrap(),
                                       dt_pool->create("str"));
    code << ")";
    o = yk_object(dt_pool->create("int"));
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┌─┐┬ ┬┌─┐┬ ┬┌┬┐
// └─┐├─┤├─┘│ │ │
// └─┘┴ ┴┴  └─┘ ┴
//
struct builtin_shput : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 3) {
      o.string_val_ = "Three arguments must be provided for shput() builtin";
    } else if (!args[0].datatype_->is_array() ||
               !args[0].datatype_->args_[0]->is_sm_entry()) {
      o.string_val_ =
          "First argument to shput() must match with Array[SMEntry[?]]";
    } else if (!args[1].datatype_->const_unwrap()->is_a_string()) {
      o.string_val_ = "Second argument to shput() must be a string";
    } else {
      auto lhs = args[0].datatype_->args_[0]->args_[0]->const_unwrap();
      auto rhs = args[2].datatype_->const_unwrap();
      auto matched =
          dt_slot_matcher->type_match(lhs, rhs, args[2].is_primitive_or_obj());
      if (matched.matched_) { return o; }
      o.string_val_ =
          "Third argument to shput() must match provided map " + matched.error_;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "yk__shput(" << args[0].first << ", ";
    dt_compiler->compile_string_assign(nullptr, code, args[1],
                                       args[1].second.datatype_->const_unwrap(),
                                       dt_pool->create("str"));
    code << ", " << args[2].first << ")";
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┌─┐┌─┐┌─┐┌┬┐
// │  ├─┤└─┐ │
// └─┘┴ ┴└─┘ ┴
//
struct builtin_cast : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for cast() builtin";
    } else if (!args[0].datatype_->const_unwrap()->is_string_literal()) {
      o.string_val_ = "First argument to cast() must be a literal.";
    } else if (!args[1].is_primitive_or_obj()) {
      o.string_val_ = "Second argument to cast() must be an object/primitive";
    } else {
      auto *lit = dynamic_cast<literal_expr *>(arg_expressions[0]);
      if (lit->literal_token_->type_ != token_type::STRING &&
          lit->literal_token_->type_ != token_type::THREE_QUOTE_STRING) {
        o.string_val_ = "First argument to cast() must be a string literal.";
      } else {
        auto data_type = lit->literal_token_->token_;
        yk_datatype *parsed_dt =
            dt_parser->parse(data_type, import_aliases, filepath);
        if (parsed_dt == nullptr) {
          o.string_val_ = "Invalid data type provided to cast()";
        } else {
          return yk_object(parsed_dt);
        }
      }
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    auto dt = dynamic_cast<literal_expr *>(arg_expressions[0]);
    auto out_dt =
        dt_parser->parse(dt->literal_token_->token_, import_aliases, filepath);
    if (out_dt->is_any_ptr() || out_dt->is_any_ptr_to_const()) {
      code << dt_compiler->wrap_in_paren(args[1].first);
    } else if (args[1].second.datatype_->is_none()) {
      code << "NULL";
    } else if (args[1].second.datatype_->const_unwrap()->is_sr()) {
      code << "(("
           << dt_compiler->convert_dt(out_dt, datatype_location::CAST, "", "")
           << ")yk__bstr_get_reference(" << args[1].first << "))";
    } else {
      code << "(("
           << dt_compiler->convert_dt(out_dt, datatype_location::CAST, "", "")
           << ")" << args[1].first << ")";
    }
    o = yk_object(out_dt);
    return {code.str(), o};
  }
  bool require_stdlib() override { return false; }
};
//
// ┬ ┬┌┬┐┌┐┌┌─┐┬ ┬
// ├─┤││││││├┤ │││
// ┴ ┴┴ ┴┘└┘└─┘└┴┘
//
struct builtin_hmnew : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 1) {
      o.string_val_ = "One argument must be provided for hmnew() builtin";
    } else if (!args[0].datatype_->is_array() ||
               !args[0].datatype_->args_[0]->is_m_entry()) {
      o.string_val_ = "Argument to hmnew() must match with Array[MEntry[K,V]]";
    } else {
      return o;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    return {"", o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┬ ┬┌┬┐┌─┐┌─┐┌┬┐
// ├─┤││││ ┬├┤  │
// ┴ ┴┴ ┴└─┘└─┘ ┴
//
struct builtin_hmget : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for hmget() builtin";
    } else if (!args[0].datatype_->is_array() ||
               !args[0].datatype_->args_[0]->is_m_entry()) {
      o.string_val_ =
          "First argument to hmget() must match with Array[MEntry[K,V]]";
    } else {
      auto lhs_raw = args[0].datatype_->args_[0]->args_[1];
      auto lhs = lhs_raw->const_unwrap();
      auto rhs = args[1].datatype_->const_unwrap();
      auto matched =
          dt_slot_matcher->type_match(lhs, rhs, args[1].is_primitive_or_obj());
      if (matched.matched_) { return yk_object(lhs_raw); }
      o.string_val_ =
          "Second argument to hmget() must match with Array[MEntry[K,V]] " +
          matched.error_;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "yk__hmget(" << args[0].first << ", " << args[1].first << ")";
    o = yk_object(args[0].second.datatype_->args_[0]->args_[0]);
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
struct builtin_hmdel : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for hmdel() builtin";
    } else if (!args[0].datatype_->is_array() ||
               !args[0].datatype_->args_[0]->is_m_entry()) {
      o.string_val_ =
          "First argument to hmdel() must match with Array[MEntry[K,V]]";
    } else {
      auto lhs_raw = args[0].datatype_->args_[0]->args_[1];
      auto lhs = lhs_raw->const_unwrap();
      auto rhs = args[1].datatype_->const_unwrap();
      auto matched =
          dt_slot_matcher->type_match(lhs, rhs, args[1].is_primitive_or_obj());
      if (matched.matched_) { return yk_object(dt_pool->create("bool")); }
      o.string_val_ =
          "Second argument to hmdel() must match with Array[MEntry[K,V]] " +
          matched.error_;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "(yk__hmdel(" << args[0].first << ", " << args[1].first << ") == 1)";
    o = yk_object(dt_pool->create("bool"));
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┬ ┬┌┬┐┌─┐┌─┐┌┬┐┬
// ├─┤││││ ┬├┤  │ │
// ┴ ┴┴ ┴└─┘└─┘ ┴ ┴
//
struct builtin_hmgeti : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for hmgeti() builtin";
    } else if (!args[0].datatype_->is_array() ||
               !args[0].datatype_->args_[0]->is_m_entry()) {
      o.string_val_ =
          "First argument to hmgeti() must match with Array[MEntry[K,V]]";
    } else {
      auto matched = dt_slot_matcher->type_match(
          args[0].datatype_->args_[0]->args_[0]->const_unwrap(),
          args[1].datatype_->const_unwrap(), args[1].is_primitive_or_obj());
      if (matched.matched_) { return yk_object(dt_pool->create("int")); }
      o.string_val_ =
          "Second argument to hmgeti() must match with Array[MEntry[K,V]] " +
          matched.error_;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "yk__hmgeti(" << args[0].first << ", " << args[1].first << ")";
    o = yk_object(dt_pool->create("int"));
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┬ ┬┌┬┐┌─┐┬ ┬┌┬┐
// ├─┤│││├─┘│ │ │
// ┴ ┴┴ ┴┴  └─┘ ┴
//
struct builtin_hmput : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 3) {
      o.string_val_ = "Three arguments must be provided for hmput() builtin";
    } else if (!args[0].datatype_->is_array() ||
               !args[0].datatype_->args_[0]->is_m_entry()) {
      o.string_val_ =
          "First argument to hmput() must match with Array[MEntry[K,V]]";
    } else {
      auto key_matched = dt_slot_matcher->type_match(
          args[0].datatype_->args_[0]->args_[0]->const_unwrap(),
          args[1].datatype_->const_unwrap(), args[1].is_primitive_or_obj());
      auto value_matched = dt_slot_matcher->type_match(
          args[0].datatype_->args_[0]->args_[1]->const_unwrap(),
          args[2].datatype_->const_unwrap(), args[2].is_primitive_or_obj());
      if (key_matched.matched_ && value_matched.matched_) { return o; }
      std::stringstream message{};
      message << "Key/Value arguments to hmput() must match with "
                 "Array[MEntry[K,V]] ";
      bool comma = false;
      if (!key_matched.matched_) {
        message << "key " << key_matched.error_;
        comma = true;
      }
      if (!value_matched.matched_) {
        if (comma) { message << ", "; }
        message << "value " << value_matched.error_;
      }
      o.string_val_ = message.str();
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "yk__hmput(" << args[0].first << ", " << args[1].first << ", "
         << args[2].first << ")";
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┌─┐ ┌─┐┌─┐┬─┐┌┬┐
// │─┼┐└─┐│ │├┬┘ │
// └─┘└└─┘└─┘┴└─ ┴
//
struct builtin_qsort : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    // TODO verify if we want to allow this for fixed size arrays
    // ?
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for sort() builtin";
    } else if (!args[0].datatype_->is_array() ||
               (args[0].datatype_->args_[0]->is_m_entry() ||
                args[0].datatype_->args_[0]->is_m_entry())) {
      o.string_val_ = "First argument to sort() must be an Array[T]";
    } else if (!(args[1].object_type_ == yaksha::object_type::FUNCTION ||
                 args[1].object_type_ == yaksha::object_type::MODULE_FUNCTION ||
                 args[1].datatype_->is_function())) {
      o.string_val_ = "Second argument to sort() must be a Function";
    } else {
      auto func = dt_pool->create("Function");
      auto func_in = dt_pool->create("In");
      auto func_out = dt_pool->create("Out");
      func->args_.emplace_back(func_in);
      func->args_.emplace_back(func_out);
      func_out->args_.emplace_back(dt_pool->create("int"));
      auto const_arg1 = dt_pool->create("Const");
      const_arg1->args_.emplace_back(dt_pool->create("AnyPtrToConst"));
      auto const_arg2 = dt_pool->create("Const");
      const_arg2->args_.emplace_back(dt_pool->create("AnyPtrToConst"));
      func_in->args_.emplace_back(const_arg1);
      func_in->args_.emplace_back(const_arg2);
      // func -> Function[In[Const[AnyArg],Const[AnyArg]],Out[int]]
      if (dt_slot_matcher->slot_match_with_result(func, args[1]).matched_) {
        return yk_object(dt_pool->create("bool"));
      } else {
        o.string_val_ =
            "Comparison must match with "
            "Function[In[Const[AnyPtrToConst],Const[AnyPtrToConst]],Out[int]]";
      }
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool->create("bool"));
    std::stringstream code{};
    o = yk_object(args[0].second.datatype_->args_[0]);
    code << "(yk__quicksort(" << args[0].first << ",sizeof("
         << dt_compiler->convert_dt(args[0].second.datatype_->args_[0],
                                    datatype_location::SIZEOF_, "", "")
         << ")" << ",yk__arrlenu(" << args[0].first << ")," << args[1].first
         << ") == 0)";
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┌─┐┬─┐┬─┐┌┐┌┌─┐┬ ┬
// ├─┤├┬┘├┬┘│││├┤ │││
// ┴ ┴┴└─┴└─┘└┘└─┘└┴┘
//
struct builtin_arrnew : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for arrnew() builtin";
    } else if (!args[0].datatype_->const_unwrap()->is_string_literal()) {
      o.string_val_ = "First argument to arrnew() must be a string literal";
    } else if (!(args[1].datatype_->const_unwrap()->is_an_integer())) {
      o.string_val_ = "Second argument to arrnew() must be an int";
    } else {
      auto *lit = dynamic_cast<literal_expr *>(arg_expressions[0]);
      auto data_type = lit->literal_token_->token_;
      yk_datatype *parsed_dt =
          dt_parser->parse(data_type, import_aliases, filepath);
      if (parsed_dt == nullptr) {
        o.string_val_ = "Invalid data type provided to arrnew()";
      } else {
        yk_datatype *array_wrapper = dt_pool->create("Array");
        array_wrapper->args_.emplace_back(parsed_dt);
        return yk_object(array_wrapper);
      }
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    auto dt = dynamic_cast<literal_expr *>(arg_expressions[0]);
    auto element_data_type =
        dt_parser->parse(dt->literal_token_->token_, import_aliases, filepath);
    auto array_dt = dt_pool->create("Array");
    array_dt->args_.emplace_back(element_data_type);
    auto array_var = st_writer->temp();
    // TODO we need to check if this is a string literal
    code << dt_compiler->convert_dt(array_dt, datatype_location::VARIABLE, "",
                                    "")
         << " " << array_var << " = NULL";
    st_writer->write_statement(code.str());
    code.str("");
    code.clear();
    code << "yk__arrsetlen(" << array_var << ", " << args[1].first << ")";
    st_writer->write_statement(code.str());
    code.str("");
    code.clear();
    code << array_var;
    o = yk_object(array_dt);
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┌─┐┬  ┐ ┬┌─┐┌┬┐┌─┐┬─┐┬─┐
// ├┤ │ ┌┴┬┘├┤  ││├─┤├┬┘├┬┘
// └  ┴ ┴ └─└─┘─┴┘┴ ┴┴└─┴└─
//
struct builtin_fixed_arr : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.empty()) {
      o.string_val_ = "fixedarr() builtin expects >= 1 arguments";
    } else if (!args[0].datatype_->const_unwrap()->is_string_literal()) {
      o.string_val_ = "First argument to fixedarr() must be a string literal";
    } else {
      auto *lit = dynamic_cast<literal_expr *>(arg_expressions[0]);
      auto data_type = lit->literal_token_->token_;
      yk_datatype *parsed_dt =
          dt_parser->parse(data_type, import_aliases, filepath);
      if (parsed_dt == nullptr) {
        o.string_val_ = "Invalid data type provided to fixedarr()";
      } else {
        size_t length = args.size();
        for (size_t i = 1; i < length; i++) {
          if (!dt_slot_matcher->slot_match_with_result(parsed_dt, args[i])
                   .matched_) {
            o.string_val_ = "All arguments must match with data type passed "
                            "to first argument for fixedarr() builtin";
            o.object_type_ = object_type::ERROR_DETECTED;
            return o;
          }
        }
        yk_datatype *array_wrapper = dt_pool->create("FixedArr");
        array_wrapper->args_.emplace_back(parsed_dt);
        // Add fixed array length
        array_wrapper->args_.emplace_back(
            dt_pool->create_dimension(static_cast<int>(length) - 1));
        array_wrapper->inlinable_literal_ = true;
        return yk_object(array_wrapper);
      }
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    auto dt = dynamic_cast<literal_expr *>(arg_expressions[0]);
    auto element_data_type =
        dt_parser->parse(dt->literal_token_->token_, import_aliases, filepath);
    auto array_dt = dt_pool->create("FixedArr");
    array_dt->args_.emplace_back(element_data_type);
    // Add fixed array length
    array_dt->args_.emplace_back(
        dt_pool->create_dimension(static_cast<int>(args.size()) - 1));
    array_dt->inlinable_literal_ = true;// this can be inlined
    // ------------------------
    code << "{";
    size_t length = args.size();
    for (size_t i = 1; i < length; i++) {
      // TODO see if this and array() require to check Const[str]?
      // TODO clearly define Const[xx] possible data types?
      // TODO should Const even be a datatype?, isn't it just a modifier?
      if (i != 1) { code << ", "; }
      if (element_data_type->is_a_string()) {
        dt_compiler->compile_string_assign(
            dt->literal_token_, code, args[i],
            args[i].second.datatype_->const_unwrap(), element_data_type);
      } else {
        code << args[i].first;
      }
    }
    code << "}";
    o = yk_object(array_dt);
    return {code.str(), o};
  }
  bool require_stdlib() override { return false; }
};
//
// ┌─┐┬─┐┬─┐┌─┐┬ ┬
// ├─┤├┬┘├┬┘├─┤└┬┘
// ┴ ┴┴└─┴└─┴ ┴ ┴
//
struct builtin_array : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.empty()) {
      o.string_val_ = "array() builtin expects >= 1 arguments";
    } else if (!args[0].datatype_->const_unwrap()->is_string_literal()) {
      o.string_val_ = "First argument to array() must be a string literal";
    } else {
      auto *lit = dynamic_cast<literal_expr *>(arg_expressions[0]);
      auto data_type = lit->literal_token_->token_;
      yk_datatype *parsed_dt =
          dt_parser->parse(data_type, import_aliases, filepath);
      if (parsed_dt == nullptr) {
        o.string_val_ = "Invalid data type provided to array()";
      } else {
        size_t length = args.size();
        for (size_t i = 1; i < length; i++) {
          if (!dt_slot_matcher->slot_match_with_result(parsed_dt, args[i])
                   .matched_) {
            o.string_val_ = "All arguments must match with data type passed "
                            "to first argument for array() builtin";
            o.object_type_ = object_type::ERROR_DETECTED;
            return o;
          }
        }
        yk_datatype *array_wrapper = dt_pool->create("Array");
        array_wrapper->args_.emplace_back(parsed_dt);
        return yk_object(array_wrapper);
      }
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    auto dt = dynamic_cast<literal_expr *>(arg_expressions[0]);
    auto element_data_type =
        dt_parser->parse(dt->literal_token_->token_, import_aliases, filepath);
    auto array_dt = dt_pool->create("Array");
    array_dt->args_.emplace_back(element_data_type);
    auto array_var = st_writer->temp();
    code << dt_compiler->convert_dt(array_dt, datatype_location::STRUCT, "", "")
         << " " << array_var << " = NULL";
    st_writer->write_statement(code.str());
    code.str("");
    code.clear();
    code << "yk__arrsetcap(" << array_var << ", " << (args.size() - 1) << ")";
    st_writer->write_statement(code.str());
    code.str("");
    code.clear();
    size_t length = args.size();
    for (size_t i = 1; i < length; i++) {
      if (element_data_type->is_a_string()) {
        code << "yk__arrput(" << array_var << ", ";
        dt_compiler->compile_string_assign(
            dt->literal_token_, code, args[i],
            args[i].second.datatype_->const_unwrap(), element_data_type);
        code << ")";
      } else {
        code << "yk__arrput(" << array_var << ", " << args[i].first << ")";
      }
      st_writer->write_statement(code.str());
      code.str("");
      code.clear();
    }
    code << array_var;
    o = yk_object(array_dt);
    return {code.str(), o};
  }
  bool require_stdlib() override { return true; }
};
//
// ┬┬┌─┐
// ││├┤
// ┴┴└
//
struct builtin_iif : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    std::stringstream message{};
    auto o = yk_object(dt_pool);
    if (args.size() != 3) {
      o.string_val_ = "iif() builtin expects 3 arguments";
    } else if (!(args[0].datatype_->const_unwrap()->is_bool())) {
      o.string_val_ = "First argument to iif() must be a bool";
    } else {
      auto matched = dt_slot_matcher->rvalue_match(args[1], args[2]);
      if (!matched.matched_) {
        message << "Second and third arguments to iif() ";
        message << matched.error_;
        o.string_val_ = message.str();
        o.object_type_ = object_type::ERROR_DETECTED;
        return o;
      }
      o = yk_object(args[1].datatype_);
      if (args[1].datatype_->const_unwrap()->is_string_literal()) {
        o.datatype_ = dt_pool->create("sr");
      }
      if (args[1].is_a_function()) {
        auto datatype_for_function =
            dt_slot_matcher->function_to_datatype_or_null(args[1]);
        if (datatype_for_function != nullptr) {
          o.datatype_ = datatype_for_function;
        } else {
          // Note: this cannot actually happen as this should error out from rvalue_match
          o.string_val_ = "Second argument to iif() must be a function";
          o.object_type_ = object_type::ERROR_DETECTED;
          return o;
        }
      }
      return o;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    if (args[1].second.datatype_->const_unwrap()->is_string_literal()) {
      o.datatype_ = dt_pool->create("sr");
      auto u_lhs = string_utils::unescape(args[1].second.string_val_);
      auto u_rhs = string_utils::unescape(args[2].second.string_val_);
      code << "(" << args[0].first << " ? yk__bstr_s(\""
           << string_utils::escape(u_lhs) << "\", " << u_lhs.size()
           << ") : " << "yk__bstr_s(\"" << string_utils::escape(u_rhs) << "\", "
           << u_rhs.size() << "))";
    } else {
      code << "(" << args[0].first << " ? " << args[1].first << " : "
           << args[2].first << ")";
      o.datatype_ = args[1].second.datatype_;
    }
    return {code.str(), o};
  }
  bool require_stdlib() override { return false; }
};
//
// ┌─┐┌─┐┬─┐┌─┐┌─┐┌─┐┬ ┬
// ├┤ │ │├┬┘├┤ ├─┤│  ├─┤
// └  └─┘┴└─└─┘┴ ┴└─┘┴ ┴
// ┌─┐┌─┐┬ ┬┌┐┌┌┬┐┬┌─┐
// │  │ ││ ││││ │ │├┤
// └─┘└─┘└─┘┘└┘ ┴ ┴└
// ┌─┐┬┬ ┌┬┐┌─┐┬─┐
// ├┤ ││  │ ├┤ ├┬┘
// └  ┴┴─┘┴ └─┘┴└─
// ┌┬┐┌─┐┌─┐
// │││├─┤├─┘
// ┴ ┴┴ ┴┴
//
struct builtin_functional : builtin {
  enum class fnc { FOREACH, COUNTIF, FILTER, MAP };
  explicit builtin_functional(fnc fnc_type, std::string name)
      : fnc_type_(fnc_type), name_(std::move(name)) {}
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    // TODO what about Const[FixedArr[...]] and Const[Array[...]]
    auto o = yk_object(dt_pool);
    if (args.size() != 3) {
      o.string_val_ = name_ + "() builtin expects 3 arguments";
    } else if ((!args[0].datatype_->is_array() &&
                !args[0].datatype_->is_fixed_size_array()) ||
               args[0].datatype_->args_[0]->is_m_entry() ||
               args[0].datatype_->args_[0]->is_sm_entry()) {
      o.string_val_ = "First argument to " + name_ + "() must be a an Array[T]";
    } else if (!(args[1].is_a_function() || args[1].datatype_->is_function())) {
      o.string_val_ = "Second argument to " + name_ + "() must be a function";
    } else {
      return verify_function(args[0], args[1], dt_slot_matcher, dt_pool,
                             args[2].datatype_);
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    std::stringstream code{};
    yk_datatype *dt;
    yk_datatype *return_val_type;
    if (args[1].second.is_a_function()) {
      dt = fnc_dt_extractor->function_to_datatype_or_null(args[1].second);
    } else {
      dt = args[1].second.datatype_;
    }
    auto fixed_array = dt->args_[0]->is_fixed_size_array();
    yk_datatype *fn_out =
        dt->args_[1]->args_[0];// Function[In[T, K], Out[O]] access O
    yk_datatype *template_dt =
        args[0].second.datatype_->args_[0];// Array[T] access T
    std::string i = st_writer->temp();
    std::string len_temp =
        fixed_array ? dt->args_[0]->token_->token_ : st_writer->temp();
    std::string arr_temp = st_writer->temp();
    std::string elm_temp = st_writer->temp();
    std::string k_temp = st_writer->temp();
    std::string return_temp = st_writer->temp();
    std::string fn_out_temp = st_writer->temp();
    std::string rt_init = "NULL";
    yk_datatype *arr1 = dt_pool->create("Array");
    // Copy pointer to array to a temp
    code << dt_compiler->convert_dt(args[0].second.datatype_,
                                    datatype_location::STRUCT, "", "")
         << " " << arr_temp << " = " << args[0].first;
    write_statement(code, st_writer);
    // Copy K value to temp
    code << dt_compiler->convert_dt(args[2].second.datatype_,
                                    datatype_location::STRUCT, "", "")
         << " " << k_temp << " = " << args[2].first;
    write_statement(code, st_writer);
    if (!fixed_array) {
      // Get length of this array
      code << "size_t " << len_temp << " = yk__arrlenu(" << arr_temp << ")";
      write_statement(code, st_writer);
    }
    // Create returning value temp
    switch (fnc_type_) {
      case fnc::FOREACH:
        return_val_type = dt_pool->create("bool");
        rt_init = "true";
        break;
      case fnc::COUNTIF:
        return_val_type = dt_pool->create("int");
        rt_init = "0";
        break;
      case fnc::FILTER:
        arr1->args_.emplace_back(template_dt);
        return_val_type = arr1;
        break;
      default:// fnc::MAP
        arr1->args_.emplace_back(fn_out);
        return_val_type = arr1;
    }
    code << dt_compiler->convert_dt(return_val_type, datatype_location::STRUCT,
                                    "", "")
         << " " << return_temp << " = " << rt_init;
    write_statement(code, st_writer);
    // For each element
    code << "for (size_t " << i << " = 0; " << i << " < " << len_temp << "; "
         << i << "++) { ";
    write_statement_no_end(code, st_writer);
    st_writer->indent();
    if (template_dt->is_str()) {
      code << dt_compiler->convert_dt(template_dt, datatype_location::STRUCT,
                                      "", "")
           << " " << elm_temp << " = yk__sdsdup(" << arr_temp << "[" << i
           << "])";
    } else {
      code << dt_compiler->convert_dt(template_dt, datatype_location::STRUCT,
                                      "", "")
           << " " << elm_temp << " = " << arr_temp << "[" << i << "]";
    }
    write_statement(code, st_writer);
    code << dt_compiler->convert_dt(fn_out, datatype_location::STRUCT, "", "")
         << " " << fn_out_temp << " = " << args[1].first << "(" << elm_temp
         << ", " << k_temp << ")";
    write_statement(code, st_writer);
    switch (fnc_type_) {
      case fnc::FOREACH:
        code << "if (!" << fn_out_temp << ") {" << return_temp
             << " = false; break; }";
        break;
      case fnc::COUNTIF:
        code << "if (" << fn_out_temp << ") {" << return_temp << "++; }";
        break;
      case fnc::FILTER:
        if (template_dt->is_str()) {
          // Create a new copy
          code << elm_temp << " = yk__sdsdup(" << arr_temp << "[" << i
               << "]); ";
          code << "if (" << fn_out_temp << ") {" << "yk__arrput(" << return_temp
               << ", " << elm_temp << "); } else {" << " yk__sdsfree("
               << elm_temp << "); }";
        } else {
          code << "if (" << fn_out_temp << ") {" << "yk__arrput(" << return_temp
               << ", " << elm_temp << "); }";
        }
        break;
      default:// fnc::MAP
        code << "yk__arrput(" << return_temp << ", " << fn_out_temp << ");";
    }
    write_statement_no_end(code, st_writer);
    st_writer->dedent();
    code << "}";
    write_statement_no_end(code, st_writer);
    // Write work code
    code.str("");
    code.clear();
    code << return_temp;
    return {code.str(), yk_object(return_val_type)};
  }
  bool require_stdlib() override { return true; }

  private:
  void write_statement(std::stringstream &read_stream,
                       statement_writer *st_writer) {
    st_writer->write_statement(read_stream.str());
    read_stream.str("");
    read_stream.clear();
  }
  void write_statement_no_end(std::stringstream &read_stream,
                              statement_writer *st_writer) {
    st_writer->write_statement_no_end(read_stream.str());
    read_stream.str("");
    read_stream.clear();
  }
  yk_object verify_function(const yk_object &array, const yk_object &function,
                            slot_matcher *dt_slot_matcher,
                            yk_datatype_pool *dt_pool,
                            yk_datatype *context_dt) {
    yk_datatype *dt;
    if (function.datatype_->is_function()) {
      dt = function.datatype_;
    } else {
      dt = dt_slot_matcher->function_to_datatype_or_null(function);
    }
    if (dt == nullptr) {
      yk_object o = yk_object(dt_pool);
      o.string_val_ = "Function argument for builtin " + name_ + " if invalid";
      o.object_type_ = object_type::ERROR_DETECTED;
      return o;
    }
    yk_object o = yk_object(dt_pool);
    yk_datatype *template_dt = array.datatype_->args_[0];
    if (dt->args_[1]->args_.size() != 1) {
      o.string_val_ = "Function argument for builtin " + name_ +
                      " () must have a return type";
    } else if (dt->args_[0]->args_.size() != 2) {
      o.string_val_ =
          "Function argument for builtin " + name_ + " () must take 2 values";
    } else if (dt_slot_matcher->is_not_identical_type(template_dt,
                                                      dt->args_[0]->args_[0])) {
      o.string_val_ = "Function argument for builtin " + name_ +
                      " () must match with provided array";
    } else if (dt_slot_matcher->is_not_identical_type(dt->args_[0]->args_[1],
                                                      context_dt)) {
      o.string_val_ = "Function argument for builtin " + name_ +
                      " () must match with context argument";
    } else if (fnc_type_ != fnc::MAP && !dt->args_[1]->args_[0]->is_bool()) {
      o.string_val_ = "Function argument for builtin " + name_ +
                      " () must return a boolean";
    } else {
      if (fnc_type_ == fnc::FOREACH) {
        o = yk_object(dt_pool->create("bool"));
      } else if (fnc_type_ == fnc::COUNTIF) {
        o = yk_object(dt_pool->create("int"));
      } else if (fnc_type_ == fnc::FILTER) {
        yk_datatype *arr = dt_pool->create("Array");
        arr->args_.emplace_back(template_dt);
        o = yk_object(arr);
      } else {
        yk_datatype *arr = dt_pool->create("Array");
        arr->args_.emplace_back(dt->args_[1]->args_[0]);
        o = yk_object(arr);
      }
      return o;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  fnc fnc_type_;
  std::string name_;
};
//
// ┌┐ ┬┌┐┌┌─┐┬─┐┬ ┬┌┬┐┌─┐┌┬┐┌─┐
// ├┴┐││││├─┤├┬┘└┬┘ ││├─┤ │ ├─┤
// └─┘┴┘└┘┴ ┴┴└─ ┴ ─┴┘┴ ┴ ┴ ┴ ┴
//
struct builtin_binarydata : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 1) {
      o.string_val_ = "binarydata() builtin expects 1 argument";
    } else if (!args[0].datatype_->const_unwrap()->is_string_literal()) {
      o.string_val_ = "Argument to binarydata() must be a str literal";
    } else {
      o = yk_object(
          dt_parser->parse("Const[Ptr[Const[u8]]]", import_aliases, filepath));
      return o;
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto str_expression = dynamic_cast<literal_expr *>(arg_expressions[0]);
    auto o = yk_object(dt_pool);
    std::string raw_string =
        string_utils::unescape(str_expression->literal_token_->token_);
    o = yk_object(
        dt_parser->parse("Const[Ptr[Const[u8]]]", import_aliases, filepath));
    return {esc->compile_binary_data(raw_string), o};
  }
  bool require_stdlib() override { return false; }
};
//
// ┌┬┐┌─┐┬┌─┌─┐
// │││├─┤├┴┐├┤
// ┴ ┴┴ ┴┴ ┴└─┘
//
// Allocate a single object of given data type on heap
//    Most common use case would be to use with a struct or a primitive
//    make("int") -> Ptr[int]
//    make("SomeStructOrClass") ->  Ptr[SomeStructOrClass]
//    Note that since classes are heap allocated already, doing this will result in 'struct MyClass **'
//    this can also make any @native structure
//
// Inspired by ac2pic's make builtin
// https://github.com/ac2pic/Yaksha/blob/main/compiler/src/builtins/builtins.cpp#L1417
struct builtin_make : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 1) {
      o.string_val_ = "One argument must be provided for make() builtin";
    } else if (!args[0].datatype_->const_unwrap()->is_string_literal()) {
      o.string_val_ = "Argument to make() must be a str literal";
    } else {
      auto *lit = dynamic_cast<literal_expr *>(arg_expressions[0]);
      auto data_type = lit->literal_token_->token_;
      yk_datatype *parsed_dt_original =
          dt_parser->parse(data_type, import_aliases, filepath);
      if (parsed_dt_original != nullptr) {
        yk_datatype *parsed_dt = parsed_dt_original->const_unwrap();
        if (parsed_dt->is_function_input() || parsed_dt->is_function_output() ||
            parsed_dt->is_m_entry() || parsed_dt->is_sm_entry() ||
            parsed_dt->is_none() || parsed_dt->is_const()) {
          o.string_val_ =
              "First argument to make() is an invalid data type " + data_type;
        } else {
          // We support things like -> i32, etc. to be created on heap as well
          yk_datatype *dt = dt_pool->create("Ptr");
          dt->args_.emplace_back(parsed_dt_original);
          return yk_object(dt);
        }
      }
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto *lit = dynamic_cast<literal_expr *>(arg_expressions[0]);
    auto data_type = lit->literal_token_->token_;
    yk_datatype *parsed_dt =
        dt_parser->parse(data_type, import_aliases, filepath);
    auto o = yk_object(dt_pool);
    std::stringstream code{};
    code << "calloc(1, sizeof("
         << dt_compiler->convert_dt(parsed_dt, datatype_location::STRUCT, "",
                                    "")
         << "))";
    yk_datatype *dt = dt_pool->create("Ptr");
    dt->args_.emplace_back(parsed_dt);
    o.datatype_ = dt;
    return {code.str(), o};
  }
  bool require_stdlib() override { return false; }
};
//
// ┬┌┐┌┬  ┬┌┐┌┌─┐┌─┐
// │││││  ││││├┤ │
// ┴┘└┘┴─┘┴┘└┘└─┘└─┘
// Arbitary inline C code.
//
// inlinec("c.CStr", "\"oi\"")
// inlinec("None", "puts(\"hello world\\n\"))
//
struct builtin_inlinec : builtin {
  yk_object
  verify(const std::vector<yk_object> &args,
         const std::vector<expr *> &arg_expressions, datatype_parser *dt_parser,
         yk_datatype_pool *dt_pool,
         const std::unordered_map<std::string, import_stmt *> &import_aliases,
         const std::string &filepath, slot_matcher *dt_slot_matcher) override {
    auto o = yk_object(dt_pool);
    if (args.size() != 2) {
      o.string_val_ = "Two arguments must be provided for inlinec() builtin";
    } else if (!args[0].datatype_->const_unwrap()->is_string_literal()) {
      o.string_val_ = "First argument to inlinec() must be a str literal";
    } else if (!args[0].datatype_->const_unwrap()->is_string_literal()) {
      o.string_val_ = "Second argument to inlinec() must be a str literal";
    } else {
      auto *lit = dynamic_cast<literal_expr *>(arg_expressions[0]);
      auto data_type = lit->literal_token_->token_;
      yk_datatype *parsed_dt_original =
          dt_parser->parse(data_type, import_aliases, filepath);
      if (parsed_dt_original != nullptr) {
        yk_datatype *parsed_dt = parsed_dt_original->const_unwrap();
        if (parsed_dt->is_function_input() || parsed_dt->is_function_output() ||
            parsed_dt->is_m_entry() || parsed_dt->is_sm_entry() ||
            parsed_dt->is_const()) {
          o.string_val_ =
              "First argument to inlinec() is an invalid data type " +
              data_type;
        } else {
          return yk_object(parsed_dt_original);
        }
      }
    }
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  std::pair<std::string, yk_object>
  compile(const std::vector<std::pair<std::string, yk_object>> &args,
          const std::vector<expr *> &arg_expressions,
          datatype_compiler *dt_compiler, datatype_parser *dt_parser,
          yk_datatype_pool *dt_pool,
          const std::unordered_map<std::string, import_stmt *> &import_aliases,
          const std::string &filepath, statement_writer *st_writer,
          function_datatype_extractor *fnc_dt_extractor,
          entry_struct_func_compiler *esc) override {
    auto *lit = dynamic_cast<literal_expr *>(arg_expressions[0]);
    auto *code = dynamic_cast<literal_expr *>(arg_expressions[1]);
    auto data_type = lit->literal_token_->token_;
    yk_datatype *parsed_dt =
        dt_parser->parse(data_type, import_aliases, filepath);
    auto o = yk_object(dt_pool);
    o.datatype_ = parsed_dt;
    return {string_utils::unescape(code->literal_token_->token_), o};
  }
  bool require_stdlib() override { return false; }
};
//=======================================
builtins::builtins(yk_datatype_pool *dt_pool, gc_pool<token> *token_pool)
    : dt_pool_{dt_pool}, builtins_{}, token_pool_(token_pool) {
  builtins_.insert({"arrput", new builtin_arrput{}});
  builtins_.insert({"arrpop", new builtin_arrpop{}});
  builtins_.insert({"arrnew", new builtin_arrnew{}});
  builtins_.insert({"arrsetcap", new builtin_arrsetlencap{"arrsetcap"}});
  builtins_.insert({"arrsetlen", new builtin_arrsetlencap{"arrsetlen"}});
  builtins_.insert({"arrdel", new builtin_arrsetlencap{"arrdel"}});
  builtins_.insert({"arrdelswap", new builtin_arrsetlencap{"arrdelswap"}});
  builtins_.insert({"arrins", new builtin_arrins{}});
  builtins_.insert({"array", new builtin_array{}});
  builtins_.insert({"fixedarr", new builtin_fixed_arr{}});
  builtins_.insert({"print", new builtin_print{"print"}});
  builtins_.insert({"println", new builtin_print{"println"}});
  builtins_.insert({"len", new builtin_len{}});
  builtins_.insert({"charat", new builtin_charat{}});
  builtins_.insert({"getref", new builtin_getref{}});
  builtins_.insert({"unref", new builtin_unref{}});
  builtins_.insert({"setref", new builtin_setref{}});
  builtins_.insert({"shnew", new builtin_shnew{}});
  builtins_.insert({"shget", new builtin_shget{}});
  builtins_.insert({"shdel", new builtin_shdel{}});
  builtins_.insert({"shgeti", new builtin_shgeti{}});
  builtins_.insert({"shput", new builtin_shput{}});
  builtins_.insert({"cast", new builtin_cast{}});
  builtins_.insert({"hmnew", new builtin_hmnew{}});
  builtins_.insert({"hmget", new builtin_hmget{}});
  builtins_.insert({"hmdel", new builtin_hmdel{}});
  builtins_.insert({"hmgeti", new builtin_hmgeti{}});
  builtins_.insert({"hmput", new builtin_hmput{}});
  builtins_.insert({"qsort", new builtin_qsort{}});
  builtins_.insert({"iif", new builtin_iif{}});
  builtins_.insert(
      {"foreach",
       new builtin_functional{builtin_functional::fnc::FOREACH, "foreach"}});
  builtins_.insert(
      {"countif",
       new builtin_functional{builtin_functional::fnc::COUNTIF, "countif"}});
  builtins_.insert({"filter", new builtin_functional{
                                  builtin_functional::fnc::FILTER, "filter"}});
  builtins_.insert(
      {"map", new builtin_functional{builtin_functional::fnc::MAP, "map"}});
  builtins_.insert({"binarydata", new builtin_binarydata{}});
  builtins_.insert({"make", new builtin_make{}});
  builtins_.insert({"inlinec", new builtin_inlinec{}});
}
builtins::~builtins() {
  for (auto &i : builtins_) { delete i.second; }
  builtins_.clear();
}
bool builtins::has_builtin(const std::string &name) {
  return builtins_.find(name) != builtins_.end();
}
yk_object builtins::verify(
    const std::string &name, const std::vector<yk_object> &args,
    const std::vector<expr *> &arg_expressions,
    const std::unordered_map<std::string, import_stmt *> &import_aliases,
    const std::string &filepath, slot_matcher *dt_slot_matcher,
    bool no_stdlib) {
  auto &builtin_object = builtins_[name];
  if (builtin_object->require_stdlib() && no_stdlib) {
    auto o = yk_object(dt_pool_);
    o.string_val_ = "Builtin '" + name + "' does not work without stdlib.";
    o.object_type_ = object_type::ERROR_DETECTED;
    return o;
  }
  return builtin_object->verify(args, arg_expressions, this, dt_pool_,
                                import_aliases, filepath, dt_slot_matcher);
}
std::pair<std::string, yk_object> builtins::compile(
    const std::string &name,
    const std::vector<std::pair<std::string, yk_object>> &args,
    const std::vector<expr *> &arg_expressions, datatype_compiler *dt_compiler,
    const std::unordered_map<std::string, import_stmt *> &import_aliases,
    const std::string &filepath, statement_writer *st_writer,
    function_datatype_extractor *fnc_dt_extractor,
    entry_struct_func_compiler *esc) {
  return builtins_[name]->compile(args, arg_expressions, dt_compiler, this,
                                  dt_pool_, import_aliases, filepath, st_writer,
                                  fnc_dt_extractor, esc);
}
yk_datatype *builtins::parse(
    std::string data_type_str,
    const std::unordered_map<std::string, import_stmt *> &import_aliases,
    const std::string &filepath) {
  auto t = tokenizer{filepath, std::move(data_type_str), token_pool_};
  t.tokenize();
  if (!t.errors_.empty()) { return nullptr; }
  auto p = parser{filepath, t.tokens_, dt_pool_};
  p.import_stmts_alias_ = import_aliases;
  yk_datatype *dt;
  try {
    dt = p.parse_datatype();
  } catch (parsing_error &ignored) {
    intentionally_ignored(ignored);
    return nullptr;
  }
  if (!p.errors_.empty()) { return nullptr; }
  p.rescan_datatypes();
  return dt;
}
#pragma clang diagnostic pop