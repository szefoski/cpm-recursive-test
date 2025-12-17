#include "project_a.hpp"
#include <fmt/format.h>

namespace project_a {

std::string ComponentA::get_info() const noexcept {
    return "ProjectA - Base Component";
}

std::string ComponentA::format_message(std:: string_view msg) const {
    return fmt::format("[ProjectA] {}", msg);
}

} // namespace project_a
