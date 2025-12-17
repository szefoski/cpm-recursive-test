#pragma once

#include "project_b.hpp"
#include <string>
#include <memory>
#include <vector>

namespace project_c {

class ComponentC {
public:
    ComponentC();
    
    [[nodiscard]] std::string get_info() const noexcept;
    
    [[nodiscard]] std::vector<std::string> get_full_dependency_chain() const;
    
    [[nodiscard]] std::string execute(std::string_view command) const;

private:
    std::unique_ptr<project_b::ComponentB> component_b_;
};

} // namespace project_c
