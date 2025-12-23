#pragma once

#include <string>
#include <string_view>

namespace hal::spi {

class Spi {
public:
  Spi() = default;

  [[nodiscard]] std::string get_info() const noexcept;

  [[nodiscard]] std::string format_message(std::string_view msg) const;
};

} // namespace hal::spi
