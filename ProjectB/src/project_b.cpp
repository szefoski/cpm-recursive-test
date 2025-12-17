#include "project_b.hpp"

namespace project_b {

ComponentB::ComponentB() 
    : component_a_(std::make_unique<project_a::ComponentA>()) {
}

std::string ComponentB::get_info() const noexcept {
    return "ProjectB - Middleware Component (uses " + component_a_->get_info() + ")";
}

std::string ComponentB::process_with_a(std::string_view input) const {
    auto formatted = component_a_->format_message(input);
    return "[ProjectB] Processed: " + formatted;
}

} // namespace project_b
