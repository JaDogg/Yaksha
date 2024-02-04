// literal_utils.cpp
#include "literal_utils.h"
#include <cinttypes>
using namespace yaksha;
#define mk_i64(x)                                                              \
  literal_number { x, 0, 0.0f, 0.0 }
#define mk_u64(x)                                                              \
  literal_number { 0, x, 0.0f, 0.0 }
#define mk_f32(x)                                                              \
  literal_number { 0, 0, x, 0.0 }
#define mk_f64(x)                                                              \
  literal_number { 0, 0, 0.0f, x }
bool can_fit_in_bits_signed(std::intmax_t number, std::size_t bits) {
  if (bits == 8) {
    return number >= INT8_MIN && number <= INT8_MAX;
  } else if (bits == 16) {
    return number >= INT16_MIN && number <= INT16_MAX;
  } else if (bits == 32) {
    return number >= INT32_MIN && number <= INT32_MAX;
  } else if (bits == 64) {
    return number >= INT64_MIN && number <= INT64_MAX;
  }
  return false;
}
bool can_fit_in_bits_unsigned(std::uintmax_t number, std::size_t bits) {
  if (bits == 8) {
    return number <= UINT8_MAX;
  } else if (bits == 16) {
    return number <= UINT16_MAX;
  } else if (bits == 32) {
    return number <= UINT32_MAX;
  } else if (bits == 64) {
    return number <= UINT64_MAX;
  }
  return false;
}
std::size_t get_bits(token_type token_type_val) {
  switch (token_type_val) {
    case token_type::INTEGER_BIN_8:
    case token_type::UINTEGER_BIN_8:
    case token_type::INTEGER_OCT_8:
    case token_type::UINTEGER_OCT_8:
    case token_type::INTEGER_DECIMAL_8:
    case token_type::UINTEGER_DECIMAL_8:
    case token_type::INTEGER_HEX_8:
    case token_type::UINTEGER_HEX_8:
      return 8;
    case token_type::INTEGER_BIN_16:
    case token_type::UINTEGER_BIN_16:
    case token_type::INTEGER_OCT_16:
    case token_type::UINTEGER_OCT_16:
    case token_type::INTEGER_DECIMAL_16:
    case token_type::UINTEGER_DECIMAL_16:
    case token_type::INTEGER_HEX_16:
    case token_type::UINTEGER_HEX_16:
      return 16;
    case token_type::INTEGER_BIN:
    case token_type::UINTEGER_BIN:
    case token_type::INTEGER_OCT:
    case token_type::UINTEGER_OCT:
    case token_type::INTEGER_DECIMAL:
    case token_type::INTEGER_HEX:
    case token_type::UINTEGER_DECIMAL:
    case token_type::UINTEGER_HEX:
      return 32;
    case token_type::INTEGER_BIN_64:
    case token_type::UINTEGER_BIN_64:
    case token_type::INTEGER_OCT_64:
    case token_type::UINTEGER_OCT_64:
    case token_type::INTEGER_DECIMAL_64:
    case token_type::UINTEGER_DECIMAL_64:
    case token_type::INTEGER_HEX_64:
    case token_type::UINTEGER_HEX_64:
      return 64;
    default:
      return 0;
  }
}
literal_conversion_result yaksha::convert_literal(token_type token_type_val,
                                                  token *literal_token) {
  size_t bits = get_bits(token_type_val);
  switch (token_type_val) {
    case token_type::INTEGER_BIN:
    case token_type::INTEGER_BIN_8:
    case token_type::INTEGER_BIN_16:
    case token_type::INTEGER_BIN_64: {
      std::string part = literal_token->token_.substr(2);
      std::intmax_t number = std::strtoimax(part.c_str(), nullptr, 2);
      std::string as_string = std::to_string(number);
      bool valid = can_fit_in_bits_signed(number, bits);
      if (!valid) {
        return {literal_type::LT_INVALID, mk_i64(0),
                "Integer literal out of range"};
      }
      return {literal_type::LT_INT, mk_i64(number), "", as_string};
    }
    case token_type::UINTEGER_BIN:
    case token_type::UINTEGER_BIN_8:
    case token_type::UINTEGER_BIN_16:
    case token_type::UINTEGER_BIN_64: {
      std::string part = literal_token->token_.substr(2);
      std::uintmax_t number = std::strtoumax(part.c_str(), nullptr, 2);
      std::string as_string = std::to_string(number);
      bool valid = can_fit_in_bits_unsigned(number, bits);
      if (!valid) {
        return {literal_type::LT_INVALID, mk_i64(0),
                "Unsigned integer literal out of range"};
      }
      return {literal_type::LT_UINT, mk_u64(number), "", as_string};
    }
    case token_type::INTEGER_OCT:
    case token_type::INTEGER_OCT_8:
    case token_type::INTEGER_OCT_16:
    case token_type::INTEGER_OCT_64: {
      std::string part = literal_token->token_.substr(2);
      std::intmax_t number = std::strtoimax(part.c_str(), nullptr, 8);
      std::string as_string = std::to_string(number);
      bool valid = can_fit_in_bits_signed(number, bits);
      if (!valid) {
        return {literal_type::LT_INVALID, mk_i64(0),
                "Integer literal out of range"};
      }
      return {literal_type::LT_INT, mk_i64(number), "", as_string};
    }
    case token_type::UINTEGER_OCT:
    case token_type::UINTEGER_OCT_8:
    case token_type::UINTEGER_OCT_16:
    case token_type::UINTEGER_OCT_64: {
      std::string part = literal_token->token_.substr(2);
      std::uintmax_t number = std::strtoumax(part.c_str(), nullptr, 8);
      std::string as_string = std::to_string(number);
      bool valid = can_fit_in_bits_unsigned(number, bits);
      if (!valid) {
        return {literal_type::LT_INVALID, mk_i64(0),
                "Unsigned integer literal out of range"};
      }
      return {literal_type::LT_UINT, mk_u64(number), "", as_string};
    }
    case token_type::INTEGER_DECIMAL:
    case token_type::INTEGER_DECIMAL_8:
    case token_type::INTEGER_DECIMAL_16:
    case token_type::INTEGER_DECIMAL_64: {
      std::string part = literal_token->token_;
      std::intmax_t number = std::strtoimax(part.c_str(), nullptr, 10);
      std::string as_string = std::to_string(number);
      bool valid = can_fit_in_bits_signed(number, bits);
      if (!valid) {
        return {literal_type::LT_INVALID, mk_i64(0),
                "Integer literal out of range"};
      }
      return {literal_type::LT_INT, mk_i64(number), "", as_string};
    }
    case token_type::UINTEGER_DECIMAL:
    case token_type::UINTEGER_DECIMAL_8:
    case token_type::UINTEGER_DECIMAL_16:
    case token_type::UINTEGER_DECIMAL_64: {
      std::string part = literal_token->token_;
      std::uintmax_t number = std::strtoumax(part.c_str(), nullptr, 10);
      std::string as_string = std::to_string(number);
      bool valid = can_fit_in_bits_unsigned(number, bits);
      if (!valid) {
        return {literal_type::LT_INVALID, mk_i64(0),
                "Unsigned integer literal out of range"};
      }
      return {literal_type::LT_UINT, mk_u64(number), "", as_string};
    }
    case token_type::INTEGER_HEX:
    case token_type::INTEGER_HEX_8:
    case token_type::INTEGER_HEX_16:
    case token_type::INTEGER_HEX_64: {
      std::string part = literal_token->token_.substr(2);
      std::intmax_t number = std::strtoimax(part.c_str(), nullptr, 16);
      std::string as_string = std::to_string(number);
      bool valid = can_fit_in_bits_signed(number, bits);
      if (!valid) {
        return {literal_type::LT_INVALID, mk_i64(0),
                "Integer literal out of range"};
      }
      return {literal_type::LT_INT, mk_i64(number), "", as_string};
    }
    case token_type::UINTEGER_HEX:
    case token_type::UINTEGER_HEX_8:
    case token_type::UINTEGER_HEX_16:
    case token_type::UINTEGER_HEX_64: {
      std::string part = literal_token->token_.substr(2);
      std::uintmax_t number = std::strtoumax(part.c_str(), nullptr, 16);
      std::string as_string = std::to_string(number);
      bool valid = can_fit_in_bits_unsigned(number, bits);
      if (!valid) {
        return {literal_type::LT_INVALID,
                mk_i64(0),
                "Unsigned integer literal out of range"};
      }
      return {literal_type::LT_UINT, mk_u64(number), "", as_string};
    }
    case token_type::FLOAT_NUMBER: {
      std::string part = literal_token->token_;
      float number = std::strtof(part.c_str(), nullptr);
      std::string as_string = std::to_string(number);
      return {literal_type::LT_F32, mk_f32(number), "", as_string};
    }
    case token_type::DOUBLE_NUMBER: {
      std::string part = literal_token->token_;
      double number = std::strtod(part.c_str(), nullptr);
      std::string as_string = std::to_string(number);
      return {literal_type::LT_F64, mk_f64(number), "", as_string};
    }
    default:
      return {literal_type::LT_INVALID, mk_i64(0), "Invalid token type"};
  }
}
