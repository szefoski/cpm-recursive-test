#include "osal.hpp"
#include <fmt/core.h>

namespace upper_layer::osal {

Osal::Osal() : crypto_(std::make_unique<hal::crypto::Crypto>()) {}

std::string Osal::get_info() const noexcept {
  int fmt_major = FMT_VERSION / 10000;
  int fmt_minor = (FMT_VERSION % 10000) / 100;
  int fmt_patch = FMT_VERSION % 100;

  return fmt::format("osal - OS Abstraction Layer\n"
                     "  |-- fmt {}.{}.{}\n"
                     "  +-- {}",
                     fmt_major, fmt_minor, fmt_patch, crypto_->get_info());
}

std::string Osal::execute(std::string_view command) const {
  auto processed = crypto_->process_with_spi(command);
  return "[osal] Final result: " + processed;
}

} // namespace upper_layer::osal
