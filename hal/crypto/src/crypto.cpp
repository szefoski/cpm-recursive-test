#include "crypto.hpp"

namespace hal::crypto {

Crypto::Crypto() : spi_(std::make_unique<hal::spi::Spi>()) {}

std::string Crypto::get_info() const noexcept {
  return "crypto - Cryptography HAL Component\n    +-- " + spi_->get_info();
}

std::string Crypto::process_with_spi(std::string_view input) const {
  auto formatted = spi_->format_message(input);
  return "[crypto] Processed: " + formatted;
}

} // namespace hal::crypto
