# JawaRaT

A cross-platform C++ tool for auditing and cracking HS256 secret keys of JSON Web Tokens (JWTs).

## Features

*   **Dictionary Attack**: Test secrets from a given wordlist.
*   **Character-Set Bruteforce**: Systematically try combinations of characters up to a defined length.
*   **alg:none Vulnerability Check**: Generates a token with `alg` set to `none` for testing.
*   **Built-in Weak Secrets Check**: Quickly tests a list of common weak passwords.
*   **JWT Information Display**: Parses and displays the header and payload of a JWT.
*   **Multi-threaded Bruteforce**: Utilizes multiple threads for faster bruteforce attacks.
*   **Cross-Platform**: Designed to compile and run on Linux and Windows.

## Prerequisites

*   A C++17 compatible compiler (e.g., GCC, Clang, MSVC).
*   CMake (version 3.10 or higher).
*   **jwt-cpp library**: For JWT manipulation.
*   **OpenSSL**: Dependency for jwt-cpp.
*   **argparse library**: For command-line argument parsing (should be available on default)

## Library Installation

### jwt-cpp & OpenSSL

**Linux (apt-get - Debian/Ubuntu):**
```bash
sudo apt-get update
sudo apt-get install libjwt-dev libssl-dev cmake g++
```

**Linux (yum - Fedora/CentOS/RHEL):**
```bash
sudo yum install jwt-devel openssl-devel cmake gcc-c++
```

**Linux (brew - macOS/Linux):**
```bash
brew install jwt-cpp openssl cmake
```
(Ensure OpenSSL from brew is correctly picked up by CMake, you might need to set `OPENSSL_ROOT_DIR`)

**Windows (using vcpkg, no visual studio):**
1. Install Visual Studio https://visualstudio.microsoft.com/vs/
2.  Install vcpkg: [https://github.com/microsoft/vcpkg](https://github.com/microsoft/vcpkg)
3.  Install libraries:

    When installing the libraries, use the Visual Studio Developer Command Prompt

    ```bash
    vcpkg install jwt-cpp openssl argparse
    ```
4.  When configuring CMake, use the vcpkg toolchain file (can be set on your settings.json if using vscode):
    ```bash
    cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
    ```

### argparse

This project assumes `argparse.hpp` is available. If somehow you dont have it, you can download it from [https://github.com/p-ranav/argparse](https://github.com/p-ranav/argparse) and place `argparse.hpp` into an `argparse` subdirectory within the project, or install it via a package manager like vcpkg. The `CMakeLists.txt` includes `argparse` as an include directory.

## Building the Project

1.  Clone the repository:
    ```bash
    git clone https://github.com/Ilyessin12/JaWaraT.git
    cd JaWaraT
    ```
2.  Create a build directory:
    ```bash
    mkdir build
    cd build
    ```
3.  Run CMake and build:
    *   **Linux/macOS:**
        ```bash
        cmake ..
        make
        ```
    *   **Windows (with vcpkg toolchain, you can set the path using .vscode/settings.json if on vscode or use an env variable within the cmake config):**
        ```bash
        cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
        cmake --build .
        ```
    *   **Windows (Visual Studio):**
        Open the project folder with Visual Studio (which has CMake support) or generate a Visual Studio solution with CMake and build it.

The executable `jawarat` (or `jawarat.exe` on Windows) will be in the `build` directory.

## Usage

```
./jawarat <jwt_token> [options]
```

or if you have added it in your system env path :
```
jawarat <jwt_token> [options]
```

**Options:**

*   `jwt`: (Required) The target JWT token to crack.
*   `-w, --wordlist <file>`: Path to wordlist file for dictionary attack.
*   `-b, --bruteforce <charset>`: Character set for brute force attack (e.g., "abcdef123").
*   `--min-len <num>`: Minimum length for brute force (default: 1).
*   `--max-len <num>`: Maximum length for brute force (default: 6).
*   `-t, --threads <num>`: Number of threads for brute force (default: 4).
*   `--alg-none`: Check for alg:none vulnerability.
*   `--info`: Display JWT header and payload information.

**Examples:**

1.  **Display JWT Information:**
    ```bash
    ./jawarat "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c" --info
    ```

2.  **Dictionary Attack:**
    ```bash
    ./jawarat "exampletoken" -w /path/to/rockyou.txt
    ```

3.  **Bruteforce Attack (lowercase letters, length 1 to 5):**
    ```bash
    ./jawarat "exampletoken2" -b "abcdefghijklmnopqrstuvwxyz" --max-len 5
    ```

4.  **Bruteforce Attack (numeric, length 4 to 6, 8 threads):**
    ```bash
    ./jawarat "example" -b "0123456789" --min-len 3 --max-len 6 -t 7
    ```

5.  **Check for alg:none vulnerability:**
    ```bash
    ./jawarat "example" --alg-none
    ```

6.  **Combined Attack (common secrets, then dictionary, then bruteforce):**
    The tool automatically tries common secrets first.
    ```bash
    ./jawarat "token" -w common_passwords.txt -b "aA1!" --max-len 4
    ```

## Tips
You can add this project into your system env path variable so that you could just type "jawarat" anywhere

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
