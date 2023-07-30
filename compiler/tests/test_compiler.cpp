#include "catch2/catch.hpp"
#include "compiler/multifile_compiler.h"
#include "file_formats/tokens_file.h"
#include "tokenizer/tokenizer.h"
#include <filesystem>
#include <string>
using namespace yaksha;
static void TEST_FILE(const std::string &A, const std::string &B,
                      const std::string &C) {
  multifile_compiler mc{};
  auto result = mc.compile(A);
  REQUIRE(result.failed_ == false);
  gc_pool<token> token_pool{};
  tokenizer c_code{"output.c", result.code_, &token_pool};
  c_code.tokenize();
  auto token_snapshot = yaksha::load_token_dump(C, &token_pool);
  yaksha::save_token_dump(C, c_code.tokens_);
  auto yaksha_file_path = std::filesystem::path{A};
  auto tokens_file_path = std::filesystem::path{C};
  std::string c_code_file =
      (tokens_file_path.parent_path() / yaksha_file_path.filename()).string() +
      ".output.c";
  std::ofstream save_file(c_code_file);
  REQUIRE(save_file.is_open() == true);
  save_file << result.code_;
  REQUIRE(c_code.tokens_.size() == token_snapshot.size());
  for (int i = 0; i < token_snapshot.size(); i++) {
    auto parsed = c_code.tokens_[i];
    auto snapshot = token_snapshot[i];
    REQUIRE(parsed->file_ == snapshot->file_);
    REQUIRE(parsed->line_ == snapshot->line_);
    REQUIRE(parsed->pos_ == snapshot->pos_);
    REQUIRE(parsed->token_ == snapshot->token_);
    REQUIRE(parsed->type_ == snapshot->type_);
  }
}
TEST_CASE("compiler: Hello World") {
  TEST_FILE("../test_data/compiler_tests/test1.yaka", "test1.yaka",
            "../test_data/compiler_tests/output/test1.tokens");
}
TEST_CASE("compiler: Defer") {
  TEST_FILE("../test_data/compiler_tests/test2.yaka", "test2.yaka",
            "../test_data/compiler_tests/output/test2.tokens");
}
TEST_CASE("compiler: Class support") {
  TEST_FILE("../test_data/compiler_tests/test3.yaka", "test3.yaka",
            "../test_data/compiler_tests/output/test3.tokens");
}
TEST_CASE("compiler: Create object from class") {
  TEST_FILE("../test_data/compiler_tests/test4.yaka", "test4.yaka",
            "../test_data/compiler_tests/output/test4.tokens");
}
TEST_CASE("compiler: Object members") {
  TEST_FILE("../test_data/compiler_tests/test5.yaka", "test5.yaka",
            "../test_data/compiler_tests/output/test5.tokens");
}
TEST_CASE("compiler: Array access") {
  TEST_FILE("../test_data/compiler_tests/test6.yaka", "test6.yaka",
            "../test_data/compiler_tests/output/test6.tokens");
}
TEST_CASE("compiler: Nested array access") {
  TEST_FILE("../test_data/compiler_tests/test7.yaka", "test7.yaka",
            "../test_data/compiler_tests/output/test7.tokens");
}
TEST_CASE("compiler: Void function") {
  TEST_FILE("../test_data/compiler_tests/voidfunc.yaka", "voidfunc.yaka",
            "../test_data/compiler_tests/output/voidfunc.tokens");
}
TEST_CASE("compiler: Native functions") {
  TEST_FILE("../test_data/compiler_tests/nativefunc.yaka", "nativefunc.yaka",
            "../test_data/compiler_tests/output/nativefunc.tokens");
}
TEST_CASE("compiler: Imports") {
  TEST_FILE("../test_data/import_tests/main.yaka", "main.yaka",
            "../test_data/import_tests/main.tokens");
}
TEST_CASE("compiler: Native functions in imports") {
  TEST_FILE("../test_data/compiler_tests/io_module_test/main.yaka", "main.yaka",
            "../test_data/compiler_tests/io_module_test/main.tokens");
}
TEST_CASE("compiler: Array methods") {
  TEST_FILE("../test_data/compiler_tests/arrstack.yaka", "arrstack.yaka",
            "../test_data/compiler_tests/output/arrstack.tokens");
}
TEST_CASE("compiler: Native defines") {
  TEST_FILE("../test_data/byol/lisp.yaka", "lisp.yaka",
            "../test_data/byol/lisp.tokens");
}
TEST_CASE("compiler: Do not copy str for getref") {
  TEST_FILE("../test_data/compiler_tests/do_not_copy_str_getref.yaka",
            "do_not_copy_str_getref.yaka",
            "../test_data/compiler_tests/output/do_not_copy_str_getref.tokens");
}
TEST_CASE("compiler: Str unref and getref hacks!") {
  TEST_FILE("../test_data/compiler_tests/str_getref_unref.yaka",
            "str_getref_unref.yaka",
            "../test_data/compiler_tests/output/str_getref_unref.tokens");
}
TEST_CASE("compiler: Test automatic generation for string hashes!") {
  TEST_FILE("../test_data/compiler_tests/string_hash.yaka", "string_hash.yaka",
            "../test_data/compiler_tests/output/string_hash.tokens");
}
TEST_CASE("compiler: Test return calls a function with defer deleted stuff !") {
  TEST_FILE("../test_data/compiler_tests/defer_return.yaka",
            "defer_return.yaka",
            "../test_data/compiler_tests/output/defer_return.tokens");
}
TEST_CASE("compiler: All @native stuff !") {
  TEST_FILE(
      "../test_data/compiler_tests/native_function_type_tests.yaka",
      "native_function_type_tests.yaka",
      "../test_data/compiler_tests/output/native_function_type_tests.tokens");
}
TEST_CASE("compiler: Test elif") {
  TEST_FILE("../test_data/compiler_tests/elif_testing.yaka",
            "elif_testing.yaka",
            "../test_data/compiler_tests/output/elif_testing.tokens");
}
TEST_CASE("compiler: Casting") {
  TEST_FILE("../test_data/compiler_tests/casting_test.yaka",
            "casting_test.yaka",
            "../test_data/compiler_tests/output/casting_test.tokens");
}
TEST_CASE("compiler: Basic function pointer") {
  TEST_FILE("../test_data/compiler_tests/function_datatype_test.yaka",
            "function_datatype_test.yaka",
            "../test_data/compiler_tests/output/function_datatype_test.tokens");
}
TEST_CASE("compiler: Function pointer passing & calling") {
  TEST_FILE("../test_data/compiler_tests/"
            "function_datatype_passing_calling_test.yaka",
            "function_datatype_passing_calling_test.yaka",
            "../test_data/compiler_tests/output/"
            "function_datatype_passing_calling_test.tokens");
}
TEST_CASE("compiler: Test automatic generation for normal hashes!") {
  TEST_FILE("../test_data/compiler_tests/normal_hash_map.yaka",
            "normal_hash_map.yaka",
            "../test_data/compiler_tests/output/normal_hash_map.tokens");
}
TEST_CASE("compiler: Test sorting functionality using qsort() !") {
  TEST_FILE("../test_data/compiler_tests/sort_test.yaka", "sort_test.yaka",
            "../test_data/compiler_tests/output/sort_test.tokens");
}
TEST_CASE("compiler: Test arrnew() !") {
  TEST_FILE("../test_data/compiler_tests/arrnew_test.yaka", "arrnew_test.yaka",
            "../test_data/compiler_tests/output/arrnew_test.tokens");
}
TEST_CASE("compiler: Test array() !") {
  TEST_FILE("../test_data/compiler_tests/array_test.yaka", "array_test.yaka",
            "../test_data/compiler_tests/output/array_test.tokens");
}
TEST_CASE("compiler: Test sorting with @native functions") {
  TEST_FILE("../test_data/compiler_tests/native_func_sort.yaka",
            "native_func_sort.yaka",
            "../test_data/compiler_tests/output/native_func_sort.tokens");
}
TEST_CASE("compiler: Test iif() builtin") {
  TEST_FILE("../test_data/compiler_tests/iif_test.yaka", "iif_test.yaka",
            "../test_data/compiler_tests/output/iif_test.tokens");
}
TEST_CASE("compiler: Test foreach() builtin") {
  TEST_FILE(
      "../test_data/compiler_tests/functional_test_foreach.yaka",
      "functional_test_foreach.yaka",
      "../test_data/compiler_tests/output/functional_test_foreach.tokens");
}
TEST_CASE("compiler: Test countif() builtin") {
  TEST_FILE(
      "../test_data/compiler_tests/functional_test_countif.yaka",
      "functional_test_countif.yaka",
      "../test_data/compiler_tests/output/functional_test_countif.tokens");
}
TEST_CASE("compiler: Test map() builtin") {
  TEST_FILE("../test_data/compiler_tests/functional_test_map.yaka",
            "functional_test_map.yaka",
            "../test_data/compiler_tests/output/functional_test_maph.tokens");
}
TEST_CASE("compiler: Test filter() builtin") {
  TEST_FILE("../test_data/compiler_tests/functional_test_filter.yaka",
            "functional_test_filter.yaka",
            "../test_data/compiler_tests/output/functional_test_filter.tokens");
}
TEST_CASE("compiler: Tuple data type") {
  TEST_FILE("../test_data/compiler_tests/tuple_test.yaka", "tuple_test.yaka",
            "../test_data/compiler_tests/output/tuple_test.tokens");
}
TEST_CASE("compiler: Number literals") {
  TEST_FILE("../test_data/compiler_tests/all_integers.yaka",
            "all_integers.yaka",
            "../test_data/compiler_tests/output/all_integers.tokens");
}
TEST_CASE("compiler: Operator test augment assign and bitwise") {
  TEST_FILE("../test_data/compiler_tests/operator_test.yaka",
            "operator_test.yaka",
            "../test_data/compiler_tests/output/operator_test.tokens");
}
TEST_CASE("compiler: Global constants") {
  TEST_FILE("../test_data/compiler_tests/global_constants.yaka",
            "global_constants.yaka",
            "../test_data/compiler_tests/output/global_constants.tokens");
}
TEST_CASE("compiler: None comparison") {
  TEST_FILE("../test_data/compiler_tests/none_comparison.yaka",
            "none_comparison.yaka",
            "../test_data/compiler_tests/output/none_comparison.tokens");
}
TEST_CASE("compiler: Loops and logical operators") {
  TEST_FILE("../test_data/compiler_tests/loops_and_logic.yaka",
            "loops_and_logic.yaka",
            "../test_data/compiler_tests/output/loops_and_logic.tokens");
}
TEST_CASE("compiler: Class stuff") {
  TEST_FILE("../test_data/compiler_tests/class_stuff.yaka", "class_stuff.yaka",
            "../test_data/compiler_tests/output/class_stuff.tokens");
}
TEST_CASE("compiler: Test binarydata() builtin") {
  TEST_FILE(
      "../test_data/compiler_tests/binarydata_builtin_test.yaka",
      "binarydata_builtin_test.yaka",
      "../test_data/compiler_tests/output/binarydata_builtin_test.tokens");
}
TEST_CASE("compiler: Test native constants") {
  TEST_FILE("../test_data/compiler_tests/native_constants.yaka",
            "native_constants.yaka",
            "../test_data/compiler_tests/output/native_constants.tokens");
}
TEST_CASE("compiler: Test @onstack class") {
  TEST_FILE("../test_data/compiler_tests/on_stack_test.yaka",
            "on_stack_test.yaka",
            "../test_data/compiler_tests/output/on_stack_test.tokens");
}
TEST_CASE("compiler: Test endless for") {
  TEST_FILE("../test_data/compiler_tests/endless_for.yaka", "endless_for.yaka",
            "../test_data/compiler_tests/output/endless_for.tokens");
}
TEST_CASE("compiler: Test println each element in for") {
  TEST_FILE("../test_data/compiler_tests/eachelem_for.yaka",
            "eachelem_for.yaka",
            "../test_data/compiler_tests/output/eachelem_for.tokens");
}
TEST_CASE("compiler: bug-fix comment break indent") {
  TEST_FILE("../test_data/bug_fixes/comment_break_indent.yaka",
            "comment_break_indent.yaka",
            "../test_data/bug_fixes/comment_break_indent.tokens");
}
TEST_CASE("compiler: macros - compile with macros!{} and dsl!{} macro usage") {
  TEST_FILE(
      "../test_data/macro_tests/eachelem_for_with_macros.yaka",
      "eachelem_for_with_macros.yaka",
      "../test_data/compiler_tests/output/eachelem_for_with_macros.tokens");
}
TEST_CASE("compiler: macros - compile time fizzbuzz!{}") {
  TEST_FILE("../test_data/macro_tests/comptime_fizz_buzz.yaka",
            "comptime_fizz_buzz.yaka",
            "../test_data/compiler_tests/output/comptime_fizz_buzz.tokens");
}
TEST_CASE("compiler: macros - load a file as a string") {
  TEST_FILE("../test_data/macro_tests/grab_file.yaka", "grab_file.yaka",
            "../test_data/compiler_tests/output/grab_file.tokens");
}
TEST_CASE("compiler: macros - different kind of arguments") {
  TEST_FILE("../test_data/macro_tests/m_args.yaka", "m_args.yaka",
            "../test_data/compiler_tests/output/m_args.tokens");
}
TEST_CASE("compiler: macros - use macros in other files in my macros") {
  TEST_FILE("../test_data/macro_tests/i_import_stuff.yaka",
            "i_import_stuff.yaka",
            "../test_data/compiler_tests/output/i_import_stuff.tokens");
}
TEST_CASE("compiler: macros - use macro from another file as module.dsl!{}") {
  TEST_FILE("../test_data/macro_tests/imported_dsl_macro_use.yaka",
            "imported_dsl_macro_use.yaka",
            "../test_data/compiler_tests/output/imported_dsl_macro_use.tokens");
}
TEST_CASE("compiler: macros - gensym usage") {
  TEST_FILE("../test_data/macro_tests/g1.yaka", "g1.yaka",
            "../test_data/compiler_tests/output/g1.tokens");
}
