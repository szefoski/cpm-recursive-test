#pragma once

#include "crypto.hpp"
#include <memory>
#include <string>
#include <vector>

namespace upper_layer::osal {

class Osal {
public:
  Osal();

  [[nodiscard]] std::string get_info() const noexcept;

  [[nodiscard]] std::string execute(std::string_view command) const;

private:
  std::unique_ptr<hal::crypto::Crypto> crypto_;
};

} // namespace upper_layer::osal