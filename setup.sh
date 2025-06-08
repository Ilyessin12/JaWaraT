#!/bin/bash

echo "Setting up JawaRaT dependencies..."

# Create third_party directory
mkdir -p third_party

# Clone jwt-cpp (header-only library)
echo "Downloading jwt-cpp..."
if [ ! -d "third_party/jwt-cpp" ]; then
    git clone https://github.com/Thalhammer/jwt-cpp.git third_party/jwt-cpp
else
    echo "jwt-cpp already exists, skipping..."
fi

# Clone argparse (header-only library)
echo "Downloading argparse..."
if [ ! -d "third_party/argparse" ]; then
    git clone https://github.com/p-ranav/argparse.git third_party/argparse
else
    echo "argparse already exists, skipping..."
fi

echo "Dependencies downloaded successfully!"
echo ""
echo "To build the project:"
echo "1. Using Make: make"
echo "2. Using CMake: "
echo "   Ensure vcpkg is installed and you have run 'vcpkg integrate install'."
echo "   Then, set the VCPKG_ROOT environment variable or replace '\$VCPKG_ROOT' with your vcpkg path:"
echo "   mkdir build && cd build && cmake .. -DCMAKE_TOOLCHAIN_FILE=\$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake && make"
echo ""
echo "Example usage:"
echo "./jawarat 'eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOmZhbHNlfQ.TJVA95OrM7E2cBab30RMHrHDcEfxjoYZgeFONFh7HgQ' --info"
echo "./jawarat 'token_here' -w rockyou.txt"
echo "./jawarat 'token_here' -b 'abcdefghijklmnopqrstuvwxyz0123456789' --max-len 8 -t 8"
