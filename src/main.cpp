#include "osal.hpp"
#include <iostream>
#include <print>

int main() {
  std::println("=== CPM Recursive Dependencies Test ===\n");
  upper_layer::osal::Osal osal;

  std::println("Osal Info:");
  std::println("{}\n", osal.get_info());

  std::println("\nExecuting command through dependency chain:");
  auto result = osal.execute("Hello from C++23!");
  std::println("{}", result);

  std::println("\n[OK] All recursive dependencies working correctly!");

  return 0;
}
