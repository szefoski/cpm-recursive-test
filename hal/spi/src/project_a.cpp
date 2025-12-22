#include "project_a.hpp"
#include <fmt/format.h>
#include <nlohmann/json.hpp>

namespace project_a {

std::string ComponentA::get_info() const noexcept {
  // Demonstrate nlohmann/json usage
  nlohmann::json info = {{"component", "ProjectA"},
                         {"description", "Base Component"},
                         {"uses", {"fmt", "nlohmann/json"}}};
  return info.dump();
}

std::string ComponentA::format_message(std::string_view msg) const {
  return fmt::format("[ProjectA] {}", msg);
}

} // namespace project_a
