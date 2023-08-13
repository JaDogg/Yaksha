#include <utility>
#include "ast/ast_printer.h"
#include "ast/parser.h"
#include "btest.h"
#include "catch2/catch.hpp"
#include "compiler/multifile_compiler.h"
#include "file_formats/tokens_file.h"
#include "tokenizer/block_analyzer.h"
#include "tokenizer/tokenizer.h"
#include "utilities/error_printer.h"
using namespace yaksha;
static void test_parser_yaka_file(const std::string &A, std::string B,
                                  const std::string &C) {
  std::ifstream code_file(A);
  if (code_file.good()) {
    std::string code((std::istreambuf_iterator<char>(code_file)),
                     std::istreambuf_iterator<char>());
    try {
      gc_pool<token> token_pool{};
      yaksha::tokenizer t(std::move(B), code, &token_pool);
      t.tokenize();
      REQUIRE(t.errors_.empty());
      block_analyzer b{t.tokens_, &token_pool};
      b.analyze();
      ykdt_pool dt_pool{};
      parser p{A, b.tokens_, &dt_pool};
      auto tree = p.parse();
      REQUIRE(!tree.empty());
      REQUIRE(p.errors_.empty());
      ast_printer prn{};
      auto ast_out = prn.print_to_str(tree);
      tokenizer lsp_code{"ast_out.l", ast_out, &token_pool};
      lsp_code.tokenize();
      auto token_snapshot = yaksha::load_token_dump(C, &token_pool);
      yaksha::save_token_dump(C, lsp_code.tokens_);
      REQUIRE(lsp_code.tokens_.size() == token_snapshot.size());
      for (int i = 0; i < token_snapshot.size(); i++) {
        auto parsed = lsp_code.tokens_[i];
        auto snapshot = token_snapshot[i];
        REQUIRE(parsed->file_ == snapshot->file_);
        REQUIRE(parsed->line_ == snapshot->line_);
        REQUIRE(parsed->pos_ == snapshot->pos_);
        REQUIRE(parsed->token_ == snapshot->token_);
        REQUIRE(parsed->type_ == snapshot->type_);
      }
    } catch (parsing_error &e) {
      DBGPRINT(e.message_);
      REQUIRE(false);
    }
  } else {
    REQUIRE(false);
  }
}
static void TEST_SNIPPET_FULL(const std::string &S, const std::string &E) {
  multifile_compiler mc{};
  const std::string &xa = S;
  auto result = mc.compile(xa, true, "dummy.yaka", ".");
  REQUIRE(result.failed_ == true);
  REQUIRE(!yaksha::errors::error_capture.empty());
  REQUIRE(yaksha::errors::has_error(E));
}
TEST_CASE("parser: Hello World") {
  test_parser_yaka_file("../test_data/compiler_tests/test1.yaka", "test1.yaka",
                        "../test_data/compiler_tests/output/test1.ast.tokens");
}
TEST_CASE("parser: Defer") {
  test_parser_yaka_file("../test_data/compiler_tests/test2.yaka", "test2.yaka",
                        "../test_data/compiler_tests/output/test2.ast.tokens");
}
TEST_CASE("parser: Class support") {
  test_parser_yaka_file("../test_data/compiler_tests/test3.yaka", "test3.yaka",
                        "../test_data/compiler_tests/output/test3.ast.tokens");
}
TEST_CASE("parser: Create object from class") {
  test_parser_yaka_file("../test_data/compiler_tests/test4.yaka", "test4.yaka",
                        "../test_data/compiler_tests/output/test4.ast.tokens");
}
TEST_CASE("parser: Object members") {
  test_parser_yaka_file("../test_data/compiler_tests/test5.yaka", "test5.yaka",
                        "../test_data/compiler_tests/output/test5.ast.tokens");
}
TEST_CASE("parser: Array access") {
  test_parser_yaka_file("../test_data/compiler_tests/test6.yaka", "test6.yaka",
                        "../test_data/compiler_tests/output/test6.ast.tokens");
}
TEST_CASE("parser: Nested array access") {
  test_parser_yaka_file("../test_data/compiler_tests/test7.yaka", "test7.yaka",
                        "../test_data/compiler_tests/output/test7.ast.tokens");
}
TEST_CASE("parser: Void function") {
  test_parser_yaka_file(
      "../test_data/compiler_tests/voidfunc.yaka", "voidfunc.yaka",
      "../test_data/compiler_tests/output/voidfunc.ast.tokens");
}
TEST_CASE("parser: Native functions") {
  test_parser_yaka_file(
      "../test_data/compiler_tests/nativefunc.yaka", "nativefunc.yaka",
      "../test_data/compiler_tests/output/nativefunc.ast.tokens");
}
TEST_CASE("parser: Unicode test") {
  test_parser_yaka_file("../test_data/unicode_test.py", "unicode_test.py",
                        "../test_data/unicode_test.py.ast.tokens");
}
TEST_CASE("parser: Negative numbers") {
  test_parser_yaka_file("../test_data/sample_negative_numbers.yaka",
                        "sample_negative_numbers.yaka",
                        "../test_data/sample_negative_numbers.yaka.ast.tokens");
}
TEST_CASE("type checker: break used outside loop") {
  TEST_SNIPPET_FULL("def main() -> int:\n"
                    "    break\n"
                    "    b: bool = False\n"
                    "    return 0",
                    "Invalid assignment target!");
}
TEST_CASE("type checker: continue used outside loop") {
  TEST_SNIPPET_FULL("continue\n"
                    "def main() -> int:\n"
                    "    c: bool = False\n"
                    "    return 0",
                    "Invalid assignment target!");
}
TEST_CASE("parser: def in def") {
  TEST_SNIPPET_FULL("def main() -> int:\n"
                    "    def x() -> None:\n"
                    "        pass\n"
                    "    c: bool = False\n"
                    "    return 0",
                    "Blocks with nested import/def/class is not supported");
}
TEST_CASE("parser: class in def") {
  TEST_SNIPPET_FULL("def main() -> int:\n"
                    "    class X:\n"
                    "        a: int\n"
                    "    c: bool = False\n"
                    "    return 0",
                    "Blocks with nested import/def/class is not supported");
}
TEST_CASE("parser: mini blocks") {
  test_parser_yaka_file(
      "../test_data/compiler_tests/smallest.yaka", "smallest.yaka",
      "../test_data/compiler_tests/output/smallest.ast.tokens");
}
