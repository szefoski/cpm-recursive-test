#include "project_c.hpp"
#include <fmt/core.h>
#include <nlohmann/json.hpp>

namespace project_c {

ComponentC::ComponentC()
    : component_b_(std::make_unique<project_b::ComponentB>()) {}

std::string ComponentC::get_info() const noexcept {
  return "ProjectC - Top-level Component (uses " + component_b_->get_info() +
         ")";
}

std::vector<std::string> ComponentC::get_full_dependency_chain() const {
  // FMT_VERSION is an integer like 120100 for version 12.1.0
  int fmt_major = FMT_VERSION / 10000;
  int fmt_minor = (FMT_VERSION % 10000) / 100;
  int fmt_patch = FMT_VERSION % 100;

  return {
      "Main Application",
      " +-> ProjectC (Top-level component)",
      fmt::format("      |-> fmt {}.{}.{} (formatting library)", fmt_major,
                  fmt_minor, fmt_patch),
      "      +-> ProjectB (Middleware component)",
      "           +-> ProjectA (Base component)",
      fmt::format("                |-> nlohmann/json {}.{}.{} (JSON library)",
                  NLOHMANN_JSON_VERSION_MAJOR, NLOHMANN_JSON_VERSION_MINOR,
                  NLOHMANN_JSON_VERSION_PATCH)};
}

std::string ComponentC::execute(std::string_view command) const {
  auto processed = component_b_->process_with_a(command);
  return "[ProjectC] Final result: " + processed;
}

} // namespace project_c
