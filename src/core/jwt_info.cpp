#include "core/jwt_info.h" // Updated path
#include "jwt-cpp/jwt.h"
#include <iostream>
#include <iomanip> // For std::setw

void print_jwt_information(const std::string& jwt_token){
    std::cout << "\n=== JWT Information ===\n";
    try{
        auto decoded = jwt::decode(jwt_token);

        std::cout << "  [+] Header:\n";
        // Iterate over header claims
        for(const auto& claim : decoded.get_header_json()){
            std::cout << "    " << std::left << std::setw(15) << claim.first << ": " << claim.second.to_str() << "\n";
        }

        std::cout << "  [+] Payload:\n";
        // Iterate over payload claims
        for(const auto& claim : decoded.get_payload_json()){
            std::cout << "    " << std::left << std::setw(15) << claim.first << ": " << claim.second.to_str() << "\n";
        }

        std::cout << "  [+] Signature: " << decoded.get_signature() << "\n";
    }
    catch(const std::exception& e){
        std::cerr << "  [-] Error decoding JWT: " << e.what() << '\n';
    }
    std::cout << "========================\n\n";
}
