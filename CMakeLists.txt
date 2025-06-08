cmake_minimum_required(VERSION 3.10)
project(JawaRaT CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# When CMAKE_TOOLCHAIN_FILE is set (e.g., via .vscode/settings.json or command line),
# find_package will search for packages in vcpkg.

# Find jwt-cpp library (provided by vcpkg)
find_package(jwt-cpp REQUIRED)

# Find OpenSSL (dependency of jwt-cpp, also provided by vcpkg)
find_package(OpenSSL REQUIRED)

# argparse is a header-only library cloned into third_party/argparse
# Its headers are expected to be in third_party/argparse/include

add_executable(jawarat
    main.cpp
    src/core/jwt_cracker.cpp
    src/core/jwt_info.cpp
    src/attacks/attack_utils.cpp
    src/attacks/dictionary_attack.cpp
    src/attacks/bruteforce_attack.cpp
)

target_include_directories(jawarat PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} # For main.cpp and third_party includes from root
    "${CMAKE_CURRENT_SOURCE_DIR}/src" # To allow #include "core/..." and #include "attacks/..."
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/argparse/include" # For argparse
)

# Link libraries using imported targets.
# These targets (jwt-cpp::jwt-cpp, OpenSSL::SSL, OpenSSL::Crypto)
# are provided by vcpkg and carry their own usage requirements (includes, libs).
target_link_libraries(jawarat PRIVATE
    jwt-cpp::jwt-cpp
    OpenSSL::SSL
    OpenSSL::Crypto
)

if(WIN32)
    # Windows specific settings, e.g., for pthreads if not using C++11 threads
    # C++11 std::thread should be fine, but if you were using a pthreads library directly:
    # find_package(Threads REQUIRED)
    # target_link_libraries(jawarat PRIVATE Threads::Threads)
endif()

# Enable compiler warnings
if(MSVC)
    target_compile_options(jawarat PRIVATE /W3)
else()
    target_compile_options(jawarat PRIVATE -Wall -Wextra -pedantic)
endif()
