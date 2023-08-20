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
// btest.h
// JaDoggx86's header only simple C++ unit testing
// At the moment we are not using testing capabilities here.
// But we can still use DBGPRINT. Which is useful for some debugging.
#ifndef BTEST_H
#define BTEST_H
#include "utilities/cpp_util.h"
#ifdef YAKSHA_TESTING
#include <iostream>
#include <sstream>
#include <string>
// colors - https://stackoverflow.com/a/30304782
#define RST "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST
#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST
#define ASSERTN(NAME, BOOL)                                                    \
  {                                                                            \
    if (!(BOOL)) {                                                             \
      failure++;                                                               \
      std::cout << "  [ " << FRED("✖") << " ] - " << KBLU << NAME << RST "\n"; \
    }                                                                          \
  }
#define DBGPRINT(EXP)                                                          \
  do {                                                                         \
    std::cout << "    DBG - " << FBLU(#EXP) << FCYN(" is ") << EXP << "\n";    \
  } while (0)
#define ASSERT(EXP) ASSERTN(#EXP, EXP)
extern int global_failures_btest;
#else
// do nothing for non testing code
#define DBGPRINT(EXP)                                                          \
  {}
#define ASSERT(EXP)                                                            \
  {}
#define ASSERTN(NAME, BOOL)                                                    \
  {}
#endif
#ifdef YAKSHA_TESTING_IMPLEMENTATION
#if defined(WIN32) || defined(_WIN32) ||                                       \
    defined(__WIN32) && !defined(__CYGWIN__)
// Reference https://stackoverflow.com/a/45588456
#include <windows.h>
class MBuf : public std::stringbuf {
  public:
  int sync() {
    fputs(str().c_str(), stdout);
    str("");
    return 0;
  }
};
#endif
int global_failures_btest = 0;
int main(int argc, char *args[]) {
#if defined(WIN32) || defined(_WIN32) ||                                       \
    defined(__WIN32) && !defined(__CYGWIN__)
  SetConsoleOutputCP(CP_UTF8);
  setvbuf(stdout, nullptr, _IONBF, 0);
  MBuf buf;
  std::cout.rdbuf(&buf);
#endif
  if (global_failures_btest > 0) {
    std::cout << FRED("---- test(s) failed ! ---- \n");
    std::cout << std::flush;
    return 1;
  } else {
    std::cout << FGRN("---- bye! ---- \n");
    std::cout << std::flush;
    return 0;
  }
}
#endif
// --------------- STATIC BLOCK --------
// ref - https://stackoverflow.com/a/34321324
#define CONCATENATE(s1, s2) s1##s2
#define EXPAND_THEN_CONCATENATE(s1, s2) CONCATENATE(s1, s2)
#ifdef __COUNTER__
#define UNIQUE_IDENTIFIER(prefix) EXPAND_THEN_CONCATENATE(prefix, __COUNTER__)
#else
#define UNIQUE_IDENTIFIER(prefix) EXPAND_THEN_CONCATENATE(prefix, __LINE__)
#endif// __COUNTER__
#define static_block STATIC_BLOCK_IMPL1(UNIQUE_IDENTIFIER(_static_block_))
#define STATIC_BLOCK_IMPL1(prefix)                                             \
  STATIC_BLOCK_IMPL2(CONCATENATE(prefix, _fn), CONCATENATE(prefix, _var))
#define STATIC_BLOCK_IMPL2(function_name, var_name)                            \
  static void function_name();                                                 \
  static int var_name __attribute((unused)) = (function_name(), 0);            \
  static void function_name()
// -------------------- STATIC
// TEST macro Usage
// TEST("name_of_test", {
//      // test code
// })
//
#ifdef YAKSHA_TESTING
#define TEST(NAME, LAMBDA_BODY)                                                \
  static_block {                                                               \
    int failure = 0;                                                           \
    LAMBDA_BODY                                                                \
    global_failures_btest += failure;                                          \
    if (failure > 0) {                                                         \
      std::cout << FRED("✖") << FMAG(" TEST:: ") << __FILE__ << ":"            \
                << __LINE__ << " " << KYEL << (NAME) << RST << "\n";           \
    } else {                                                                   \
      std::cout << FGRN("✔") << FMAG(" TEST:: ") << __FILE__ << ":"            \
                << __LINE__ << " " << KYEL << (NAME) << RST << "\n";           \
    }                                                                          \
  }
#else
#define TEST(NAME, LAMBDA_BODY)
#endif
#endif
