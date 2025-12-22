#pragma once

#include <string>
#include <string_view>

namespace project_a {

class ComponentA {
public:
    ComponentA() = default;
    
    [[nodiscard]] std::string get_info() const noexcept;
    
    [[nodiscard]] std::string format_message(std::string_view msg) const;
};

} // namespace project_a
