#!/bin/bash

set -e

echo "========================================"
echo "CPM Recursive Dependency Test - Builder"
echo "========================================"
echo ""

GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}Creating build directory...${NC}"
mkdir -p build
cd build

echo -e "${BLUE}Configuring with CMake...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=Release

echo -e "${BLUE}Building project...${NC}"
cmake --build .  -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo ""
echo -e "${GREEN}Build successful!${NC}"
echo ""
echo "========================================"
echo "Running application:"
echo "========================================"
./main

echo ""
echo -e "${GREEN}✓ Test completed successfully!${NC}"
