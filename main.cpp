#include "project_c.hpp"
#include <iostream>
#include <print>

int main() {
    std::println("=== CPM Recursive Dependencies Test ===\n");
    
    project_c::ComponentC component;
    
    std::println("Component Info:");
    std::println("{}\n", component.get_info());
    
    std::println("Full Dependency Chain:");
    for (const auto& dep : component. get_full_dependency_chain()) {
        std::println("{}", dep);
    }
    
    std::println("\nExecuting command through dependency chain:");
    auto result = component.execute("Hello from C++23!");
    std::println("{}", result);
    
    std::println("\n✓ All recursive dependencies working correctly!");
    
    return 0;
}
