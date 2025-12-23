#pragma once

#include "spi.hpp"
#include <memory>
#include <string>

namespace hal::crypto {

class Crypto {
public:
  Crypto();

  [[nodiscard]] std::string get_info() const noexcept;

  [[nodiscard]] std::string process_with_spi(std::string_view input) const;

private:
  std::unique_ptr<hal::spi::Spi> spi_;
};

} // namespace hal::crypto
