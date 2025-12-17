#include "project_c.hpp"

namespace project_c {

ComponentC::ComponentC() 
    : component_b_(std::make_unique<project_b::ComponentB>()) {
}

std::string ComponentC::get_info() const noexcept {
    return "ProjectC - Top-level Component (uses " + component_b_->get_info() + ")";
}

std::vector<std:: string> ComponentC::get_full_dependency_chain() const {
    return {
        "Main Application",
        "-> ProjectC (Top-level)",
        "   -> ProjectB (Middleware)",
        "      -> ProjectA (Base)",
        "         -> fmt (External)"
    };
}

std::string ComponentC::execute(std::string_view command) const {
    auto processed = component_b_->process_with_a(command);
    return "[ProjectC] Final result: " + processed;
}

} // namespace project_c
