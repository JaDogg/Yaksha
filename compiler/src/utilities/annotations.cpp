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
// annotations.cpp
#include "utilities/annotations.h"
using namespace yaksha;
void annotations::validate_native_arg(std::string &arg, bool arg_set) {
  if (arg.empty() && arg_set) {
    error_ =
        "Empty string cannot be used as argument to @native or @nativemacro.";
  }
}
void annotations::validate_template(std::string &arg, bool arg_set) {
  if (!arg_set || arg.empty()) {
    error_ = "Argument must be set for @template with valid template types. "
             "Argument cannot be empty as well.";
  }
}
void annotations::add(const annotation &a) {
  if (a.name_ == "native") {
    if (native_) {
      error_ = "Duplicate annotation: @native";
      return;
    }
    if (native_macro_) {
      error_ = "Cannot use @nativemacro when @native is present";
      return;
    }
    if (native_define_) {
      error_ = "Cannot use @nativedefine when @native is present";
      return;
    }
    native_ = true;
    native_arg_ = a.arg_;
    validate_native_arg(native_arg_, a.arg_set_);
    if (!error_.empty()) return;
  } else if (a.name_ == "nativemacro") {
    if (native_macro_) {
      error_ = "Duplicate annotation: @nativemacro";
      return;
    }
    if (native_) {
      error_ = "Cannot use @native when @nativemacro is present";
      return;
    }
    if (native_define_) {
      error_ = "Cannot use @native when @nativedefine is present";
      return;
    }
    native_macro_ = true;
    native_macro_arg_ = a.arg_;
    validate_native_arg(native_macro_arg_, a.arg_set_);
    if (!error_.empty()) return;
  } else if (a.name_ == "nativedefine") {
    if (native_define_) {
      error_ = "Duplicate annotation: @nativedefine";
      return;
    }
    if (native_) {
      error_ = "Cannot use @native when @nativedefine is present";
      return;
    }
    if (native_macro_) {
      error_ = "Cannot use @nativemacro when @nativedefine is present";
      return;
    }
    native_define_ = true;
    native_define_arg_ = a.arg_;
    validate_native_arg(native_define_arg_, a.arg_set_);
    if (native_define_arg_.empty()) {
      error_ = "@nativedefine must have an argument";
      native_define_ = false;
      native_define_arg_ = "";
      return;
    }
    if (!error_.empty()) return;
  } else if (a.name_ == "template") {
    if (template_) {
      error_ = "Duplicate annotation: @template";
      return;
    }
    template_ = true;
    template_arg_ = a.arg_;
    validate_template(template_arg_, a.arg_set_);
    if (!error_.empty()) return;
  } else if (a.name_ == "varargs") {
    if (varargs_) {
      error_ = "Duplicate annotation: @varargs";
      return;
    }
    if (a.arg_set_) {
      error_ = "@vararg does not allow any argument";
      return;
    }
    varargs_ = true;
  } else if (a.name_ == "onstack") {
    if (on_stack_) {
      error_ = "Duplicate annotation: @onstack";
      return;
    }
    if (a.arg_set_) {
      error_ = "@onstack does not allow any argument";
      return;
    }
    on_stack_ = true;
  } else {
    error_ = "Invalid annotation:" + a.name_;
    return;
  }
  annotations_.emplace_back(a);
}
