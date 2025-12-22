#pragma once

#include "project_a.hpp"
#include <string>
#include <memory>

namespace project_b {

class ComponentB {
public: 
    ComponentB();
    
    [[nodiscard]] std::string get_info() const noexcept;
    
    [[nodiscard]] std::string process_with_a(std::string_view input) const;

private:
    std::unique_ptr<project_a::ComponentA> component_a_;
};

} // namespace project_b
