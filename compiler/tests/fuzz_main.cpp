#include "ast/ast_printer.h"
#include "ast/ast_vis.h"
#include "ast/parser.h"
#include "compiler/multifile_compiler.h"
#include "compiler/type_checker.h"
#include "file_formats/tokens_file.h"
#include "tokenizer/block_analyzer.h"
#include "tokenizer/tokenizer.h"
#include "utilities/error_printer.h"
using namespace yaksha;
void test_ast(const std::string &data, const std::string &file_name) {
  gc_pool<token> token_pool{};
  tokenizer t{file_name, data, &token_pool};
  ykdt_pool dt_pool{};
  t.tokenize();
  if (!t.errors_.empty()) {
    errors::print_errors(t.errors_);
    return;
  }
  block_analyzer b{t.tokens_, &token_pool};
  b.analyze();
  try {
    parser p{file_name, b.tokens_, &dt_pool};
    auto tree = p.parse();
    if (!tree.empty()) {
      ast_printer pr{};
      pr.print(tree);
      std::cout << "\n\n============================\n";
      ast_vis vr{};
      vr.print(tree);
    } else {
      errors::print_errors(p.errors_);
      return;
    }
  } catch (parsing_error &p) {
    std::cout << "Parsing error --> " << p.message_ << "\n";
    write_token_dump(std::cerr, b.tokens_);
    return;
  }
}
void test_compiler(const std::string &filepath) {
  multifile_compiler mc{};
  auto result = mc.compile(filepath, "/app/libs");
  std::cout << "Success : " << (result.failed_ ? "No\n" : "Yes\n");
  std::cout << result.code_ << "\n";
}
#ifdef YAKSHA_LLVM_FUZZ
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  std::string code{(char *) Data, Size};
  std::string fname = "fuzz_dummy.yaka";
  std::string lib_path = "/app/libs";
  test_ast(code, fname);
  multifile_compiler mc{};
  mc.compile(code, true, fname, lib_path);
  return 0;
}
#else
int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: Yaksha script.yaka\n";
    return EXIT_SUCCESS;
  }
  std::string file_name{argv[1]};
  std::ifstream script_file(file_name);
  if (!script_file.good()) {
    std::cerr << "Failed to read file:" << file_name << "\n";
    return EXIT_SUCCESS;
  }
  std::string data((std::istreambuf_iterator<char>(script_file)),
                   std::istreambuf_iterator<char>());
  test_compiler(argv[1]);
  test_ast(data, file_name);
  return EXIT_SUCCESS;
}
#endif
