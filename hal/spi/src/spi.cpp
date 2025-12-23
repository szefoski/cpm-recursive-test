#include "spi.hpp"
#include <fmt/format.h>
#include <nlohmann/json.hpp>

namespace hal::spi {

std::string Spi::get_info() const noexcept {
  int fmt_major = FMT_VERSION / 10000;
  int fmt_minor = (FMT_VERSION % 10000) / 100;
  int fmt_patch = FMT_VERSION % 100;

  return fmt::format("spi - SPI HAL Component\n      "
                     "|-- fmt {}.{}.{}\n      "
                     "+-- nlohmann/json {}.{}.{}",
                     fmt_major, fmt_minor, fmt_patch,
                     NLOHMANN_JSON_VERSION_MAJOR, NLOHMANN_JSON_VERSION_MINOR,
                     NLOHMANN_JSON_VERSION_PATCH);
}

std::string Spi::format_message(std::string_view msg) const {
  return fmt::format("[spi] {}", msg);
}

} // namespace hal::spi
