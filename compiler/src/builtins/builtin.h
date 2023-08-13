// builtin.h
#ifndef BUILTIN_H
#define BUILTIN_H
#include "compiler/datatype_compiler.h"
#include "compiler/datatype_parser.h"
#include "compiler/entry_struct_func_compiler.h"
#include "compiler/function_datatype_extractor.h"
#include "compiler/slot_matcher.h"
#include "compiler/statement_writer.h"
#include "utilities/ykobject.h"
#include <string>
#include <unordered_map>
#include <vector>
namespace yaksha {
  struct builtin {
    virtual ~builtin() = default;
    virtual ykobject
    verify(const std::vector<ykobject> &args,
           const std::vector<expr *> &arg_expressions,
           datatype_parser *dt_parser, ykdt_pool *dt_pool,
           const std::unordered_map<std::string, import_stmt *> &import_aliases,
           const std::string &filepath, slot_matcher *dt_slot_matcher) = 0;
    virtual std::pair<std::string, ykobject> compile(
        const std::vector<std::pair<std::string, ykobject>> &args,
        const std::vector<expr *> &arg_expressions,
        datatype_compiler *dt_compiler, datatype_parser *dt_parser,
        ykdt_pool *dt_pool,
        const std::unordered_map<std::string, import_stmt *> &import_aliases,
        const std::string &filepath, statement_writer *st_writer,
        function_datatype_extractor *fnc_dt_extractor,
        entry_struct_func_compiler *esc) = 0;
  };
}// namespace yaksha
#endif
