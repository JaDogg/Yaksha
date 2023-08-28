// ==============================================================================================
// ╦  ┬┌─┐┌─┐┌┐┌┌─┐┌─┐    Yaksha Programming Language
// ║  ││  ├┤ │││└─┐├┤     is Licensed with GPLv3 + exta terms. Please see below.
// ╩═╝┴└─┘└─┘┘└┘└─┘└─┘
// Note: libs - MIT license, runtime/3rd - various
// ==============================================================================================
// GPLv3:
//
// Yaksha - Programming Language.
// Copyright (C) 2020 - 2023 Bhathiya Perera
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
// multifile_compiler.cpp
#include "multifile_compiler.h"
#include "ast/codefiles.h"
#include "ast/parser.h"
#include "compiler/compiler.h"
#include "compiler/type_checker.h"
#include "tokenizer/block_analyzer.h"
#include "usage_analyser.h"
#include "utilities/error_printer.h"
using namespace yaksha;
multifile_compiler_result
multifile_compiler::compile(const std::string &main_file) {
  LOG_COMP("compile:" << main_file);
  auto libs_path = std::filesystem::path{get_my_exe_path()}.parent_path();
  libs_path = libs_path.parent_path() / "libs";
  return compile(main_file, libs_path);
}
//
multifile_compiler_result
multifile_compiler::compile(const std::string &main_file,
                            const std::string &libs_path) {
  std::string empty_code{};
  LOG_COMP("compile:" << main_file);
  return compile(empty_code, false, main_file, libs_path);
}
// With input code
multifile_compiler_result
multifile_compiler::compile(const std::string &code, bool use_code,
                            const std::string &main_file,
                            const std::string &libs_path) {
  LOG_COMP("compile:" << main_file);
  std::filesystem::path library_parent{libs_path};
  codefiles cf{library_parent};
  // Step 0) First of all, we initialize parsing
  // In this step, we initialize all files we know of at this point
  file_info *main_file_info;
  if (!use_code) {
    main_file_info = cf.initialize_parsing_or_null(main_file);
  } else {
    main_file_info = cf.initialize_parsing_or_null(code, main_file);
  }
  LOG_COMP("init parsing");
  if (main_file_info == nullptr) {
    std::cerr << "Failed to initialize parsing:" << main_file << "\n";
    return {true, ""};
  }
  bool all_files_all_successful = false;
  while (!all_files_all_successful) {
    step_1_scan_macro_soup(cf);
    LOG_COMP("macro soup scanned");
    step_2_initialize_preprocessor_env(cf);
    LOG_COMP("preprocessor init");
    step_3_macros_setup(cf);
    LOG_COMP("setup macros!");
    step_4_expand_macros(cf);
    LOG_COMP("expand");
    step_5_parse(cf);
    LOG_COMP("parse");
    bool should_bail = step_6_rescan_imports(cf, main_file_info);
    if (should_bail) {
      LOG_COMP("rescan failed");
      return {true, ""};
    }
    step_7_verify_import_rescan_done(cf);
    LOG_COMP("rescan done");
    should_bail = has_any_failures(cf);
    if (should_bail) {
      LOG_COMP("parsing failures found");
      return {true, ""};
    }
    all_files_all_successful = all_success(cf);
  }
  return compile_all(cf, main_file_info);
}
bool multifile_compiler::has_any_failures(codefiles &cf) const {
  bool should_bail = false;
  for (auto f : cf.files_) {
    if (f->step_ == scanning_step::FAILURE) {
      LOG_COMP("has failure:" << f->filepath_.string());
      should_bail = true;
      break;
    }
  }
  return should_bail;
}
bool multifile_compiler::all_success(codefiles &cf) const {
  LOG_COMP("verify if parsing is successful");
  bool all_ok = true;
  for (auto f : cf.files_) {
    if (f->step_ != scanning_step::ALL_PARSING_SUCCESSFUL) {
      all_ok = false;
      break;
    }
  }
  return all_ok;
}
void multifile_compiler::step_7_verify_import_rescan_done(codefiles &cf) const {
  LOG_COMP("verify rescan completed");
  for (auto f : cf.files_) {
    if (f->step_ == scanning_step::FAILURE ||
        f->step_ < scanning_step::PARSE_DONE) {
      continue;
    }
    bool all_imports_ok = true;
    for (auto imp : f->data_->parser_->import_stmts_) {
      if (imp->data_ == nullptr ||
          imp->data_->step_ < scanning_step::PARSE_DONE) {
        all_imports_ok = false;
        break;
      }
    }
    if (all_imports_ok) { f->step_ = scanning_step::ALL_PARSING_SUCCESSFUL; }
  }
}
bool multifile_compiler::step_6_rescan_imports(
    codefiles &cf, file_info *main_file_info) const {
  LOG_COMP("rescan imports");
  bool should_bail = false;
  for (auto imp : main_file_info->data_->parser_->import_stmts_) {
    auto import_data = cf.scan_or_null(imp);
    if (import_data == nullptr) {
      std::cerr << "Failed to rescan imports:" << main_file_info->filepath_
                << "\n";
      should_bail = true;
      break;
    }
    imp->data_ = import_data;
  }
  return should_bail;
}
multifile_compiler_result
multifile_compiler::compile_all(codefiles &cf, file_info *main_file_info) {
  LOG_COMP("compile all");
  bool has_errors = false;
  // Extract defs and structs
  for (auto f : cf.files_) {
    LOG_COMP("file:" << f->filepath_.string());
    auto builtins_obj = new builtins(&cf.pool_, &token_pool_);
    f->data_->dsv_ = new def_class_visitor(builtins_obj);
    f->data_->dsv_->extract(f->data_->parser_->stmts_);
    LOG_COMP("dsv extract:" << f->filepath_.string());
    if (!f->data_->dsv_->errors_.empty()) {
      LOG_COMP("dsv errors");
      errors::print_errors(f->data_->dsv_->errors_);
      has_errors = true;
    }
    delete builtins_obj;
  }
  LOG_COMP("post loop");
  has_errors = has_invalid_main_func(main_file_info);
  if (has_errors) {
    LOG_COMP("has invalid main");
    return {true, ""};
  }
  LOG_COMP("has a main()");
  // Ensure all data types have the proper module
  for (auto f : cf.files_) { f->data_->parser_->rescan_datatypes(); }
  // Type check all files
  for (auto f : cf.files_) {
    f->data_->type_checker_ = new type_checker(
        f->filepath_.string(), &cf, f->data_->dsv_, &cf.pool_, &token_pool_);
    for (auto impo : f->data_->parser_->import_stmts_) {
      auto obj = ykobject(&cf.pool_);
      obj.object_type_ = object_type::MODULE;
      obj.string_val_ = impo->data_->filepath_.string();
      obj.module_file_ = impo->data_->filepath_.string();
      obj.module_name_ = impo->name_->token_;
      f->data_->type_checker_->scope_.define_global(impo->name_->token_, obj);
    }
    f->data_->type_checker_->check(f->data_->parser_->stmts_);
    if (!f->data_->type_checker_->errors_.empty()) {
      errors::print_errors(f->data_->type_checker_->errors_);
      LOG_COMP("type checker found errors: " << f->filepath_.string());
      has_errors = true;
    }
  }
  if (has_errors) {
    LOG_COMP("found type checking errors");
    return {true, ""};
  }
  // Statement usage analysis
  // So we know which 'functions / classes / consts' are actually used
  usage_analyser ua{main_file_info};
  ua.analyse();
  if (!ua.errors_.empty()) {
    errors::print_errors(ua.errors_);
    LOG_COMP("usage analyser found errors");
    return {true, ""};
  }
  // Compile all files.
  std::stringstream struct_forward_decls{};
  std::stringstream function_forward_decls{};
  std::stringstream struct_body{};
  std::stringstream function_body{};
  std::stringstream global_consts{};
  std::unordered_set<std::string> runtime_features{};
  std::vector<parsing_error> compiler_errors_{};
  int file_count = static_cast<int>(cf.files_.size());
  for (int i = file_count - 1; i >= 0; i--) {
    auto f = cf.files_[i];
    compiler c{*f->data_->dsv_, &cf.pool_, cf.esc_, &token_pool_};
    auto result = c.compile(&cf, f);
    struct_forward_decls << result.struct_forward_declarations_;
    function_forward_decls << result.function_forward_declarations_;
    struct_body << result.classes_;
    function_body << result.body_;
    global_consts << result.global_constants_;
    for (const std::string &feature : f->data_->dsv_->runtime_features_) {
      runtime_features.insert(feature);
    }
    for (const auto &err : result.errors_) {
      compiler_errors_.emplace_back(err);
      has_errors = true;
    }
  }
  // We found errors during compile time
  if (has_errors) {
    errors::print_errors(compiler_errors_);
    LOG_COMP("found compile time errors");
    return {true, ""};
  }
  std::vector<std::string> rf{};
  rf.insert(rf.end(), runtime_features.begin(), runtime_features.end());
  std::sort(rf.begin(), rf.end());
  std::stringstream c_code{};
  // Write the feature requirements
  // ---
  // Format: // YK --> no requirements
  // Format: // YK:a,b,c# --> 'a', 'b', 'c' are requirements
  // Format: // YK:a# --> 'a' is the only requirement
  c_code << "// YK";
  if (!rf.empty()) {
    c_code << ":";
    bool first = true;
    for (auto &feature : rf) {
      if (first) {
        first = false;
      } else {
        c_code << ",";
      }
      c_code << feature;
    }
    c_code << "#";
  }
  c_code << "\n#include \"yk__lib.h\"\n";
  c_code << "// --forward declarations-- \n";
  c_code << global_consts.str();
  if (cf.esc_->has_bin_data()) { cf.esc_->compile_binary_data_to(c_code); }
  c_code << struct_forward_decls.str();
  if (cf.esc_->has_structures()) {
    cf.esc_->compile_forward_declarations(c_code);
  }
  if (cf.esc_->has_functions()) { cf.esc_->compile_function_defs(c_code); }
  c_code << function_forward_decls.str();
  c_code << "// --structs-- \n";
  c_code << struct_body.str();
  if (cf.esc_->has_structures()) { cf.esc_->compile_structures(c_code); }
  c_code << "// --functions-- \n";
  c_code << function_body.str();
  c_code << "#if defined(YK__MINIMAL_MAIN)\n";
  c_code << "int main(void) { return yy__main(); }\n";
  c_code << "#endif";
  LOG_COMP("c code generated");
  return {false, c_code.str()};
}
void multifile_compiler::step_5_parse(codefiles &cf) {
  LOG_COMP("parsing: parsing to yaksha AST");
  // Step 5) Parse the file
  for (auto f : cf.files_) {
    if (f->step_ == scanning_step::FAILURE ||
        f->step_ >= scanning_step::PARSE_DONE) {
      continue;
    }
    f->data_->parser_->parse();
    if (f->data_->parser_->errors_.empty()) {
      f->step_ = scanning_step::PARSE_DONE;
    } else {
      std::cerr << "Failed to parse:" << f->filepath_ << "\n";
      errors::print_errors(f->data_->parser_->errors_);
      f->step_ = scanning_step::FAILURE;
    }
  }
}
void multifile_compiler::step_4_expand_macros(codefiles &cf) {
  LOG_COMP("parsing: dsl macro expansion");
  // Step 4) Expand macros
  for (auto f : cf.files_) {
    if (f->step_ == scanning_step::FAILURE ||
        f->step_ >= scanning_step::MACROS_EXPANDED) {
      continue;
    }
    f->data_->parser_->step_4_expand_macros(&cf.yaksha_macros_, &token_pool_);
    if (f->data_->parser_->errors_.empty()) {
      f->step_ = scanning_step::MACROS_EXPANDED;
    } else {
      std::cerr << "Failed to expand dsl!{} in lisp-macro env:" << f->filepath_
                << "\n";
      errors::print_errors(f->data_->parser_->errors_);
      f->step_ = scanning_step::FAILURE;
    }
  }
}
void multifile_compiler::step_3_macros_setup(codefiles &cf) {
  LOG_COMP("parsing: macros setup by executing all macros statements");
  // Step 3) Macros setup by executing all macros statements
  for (auto f : cf.files_) {
    if (f->step_ == scanning_step::FAILURE ||
        f->step_ >= scanning_step::MACROS_SETUP_DONE) {
      continue;
    }
    f->data_->parser_->step_3_excute_macros(&cf.yaksha_macros_);
    if (f->data_->parser_->errors_.empty()) {
      f->step_ = scanning_step::MACROS_SETUP_DONE;
    } else {
      std::cerr << "Failed to execute macros!{} in lisp-macro env:"
                << f->filepath_ << "\n";
      errors::print_errors(f->data_->parser_->errors_);
      f->step_ = scanning_step::FAILURE;
    }
  }
}
void multifile_compiler::step_2_initialize_preprocessor_env(codefiles &cf) {
  LOG_COMP("parsing: initialize preprocessing lisp environments for each file");
  // Step 2) initialize preprocessing lisp environments for each file
  for (auto f : cf.files_) {
    if (f->step_ == scanning_step::FAILURE ||
        f->step_ >= scanning_step::ENV_CREATED) {
      continue;
    }
    std::string fp = f->filepath_.string();
    LOG_COMP("parsing: initialize preprocessing lisp environments for " << fp);
    try {
      cf.yaksha_macros_.init_env(fp, f->data_->parser_->import_stmts_alias_);
      f->step_ = scanning_step::ENV_CREATED;
    } catch (const parsing_error &ex) { /* redefining imports, etc */
      std::cerr << "Failed to initialize lisp-macro env:" << f->filepath_
                << "\n";
      errors::print_error(std::cerr, ex);
      f->step_ = scanning_step::FAILURE;
    }
  }
}
void multifile_compiler::step_1_scan_macro_soup(codefiles &cf) {
  LOG_COMP("parsing: scan macro soup");
  // Step 1) Scan macro soup
  for (auto f : cf.files_) {
    if (f->step_ == scanning_step::FAILURE ||
        f->step_ >= scanning_step::SOUP_SCAN_DONE) {
      continue;
    }
    f->data_->parser_->step_1_parse_token_soup();
    if (f->data_->parser_->errors_.empty()) {
      f->step_ = scanning_step::SOUP_SCAN_DONE;
    } else {
      std::cerr << "Failed to parse non-preprocessed tokens:" << f->filepath_
                << "\n";
      errors::print_errors(f->data_->parser_->errors_);
      f->step_ = scanning_step::FAILURE;
    }
  }
}
bool multifile_compiler::has_invalid_main_func(
    file_info *main_file_info) const {
  LOG_COMP("checking do we have a main()");
  bool has_errors = false;
  if (main_file_info->data_->dsv_->has_function("main")) {
    auto main_function = main_file_info->data_->dsv_->get_function("main");
    if (!main_function->params_.empty()) {
      std::cerr << "Critical !! main() function must not have parameters";
      has_errors = true;
    }
    if (!main_function->return_type_->is_i32()) {
      std::cerr << "Critical !! main() function must return an integer";
      has_errors = true;
    }
  } else {
    std::cerr << "Critical !! main() function must be present";
    has_errors = true;
  }
  return has_errors;
}
