// Note this file only has things to compile during TESTING
#include "utilities/error_printer.h"
#ifdef YAKSHA_TESTING
namespace yaksha::errors {
  std::vector<std::string> error_capture{};
}
#endif
