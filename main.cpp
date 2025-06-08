#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iomanip>
#include "argparse/argparse.hpp"

#include "src/core/jwt_cracker.h"
#include "src/core/jwt_info.h"
#include "src/attacks/attack_utils.h"
#include "src/attacks/dictionary_attack.h"
#include "src/attacks/bruteforce_attack.h"

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("jawarat", "1.0");
    
    program.add_argument("jwt")
        .help("Target JWT token to crack");
    
    program.add_argument("-w", "--wordlist")
        .help("Path to wordlist file for dictionary attack");
    
    program.add_argument("-b", "--bruteforce")
        .help("Character set for brute force attack")
        .default_value(std::string(""));
    
    program.add_argument("--min-len")
        .help("Minimum length for brute force")
        .scan<'i', int>()
        .default_value(1);
    
    program.add_argument("--max-len")
        .help("Maximum length for brute force")
        .scan<'i', int>()
        .default_value(6);
    
    program.add_argument("-t", "--threads")
        .help("Number of threads for brute force")
        .scan<'i', int>()
        .default_value(4);
    
    program.add_argument("--alg-none")
        .help("Check for alg:none vulnerability")
        .flag();
    
    program.add_argument("--info")
        .help("Display JWT information")
        .flag();
    
    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err){
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }
    
    std::string jwt_token_str = program.get<std::string>("jwt");
    
    std::cout << "JWT Secret Key Auditor & Cracker v1.0\n";
    std::cout << "=====================================\n";
    
    try {
        JWTCracker cracker(jwt_token_str);
        
        if(program.get<bool>("info")){
            print_jwt_information(cracker.getTargetJWT());
        }
        
        if(program.get<bool>("alg-none")){
            check_alg_none_vulnerability(cracker.getTargetJWT());
        }
        
        // Always check common secrets first
        if(check_common_secrets_attack(cracker)){
            std::cout << "\n[+] Use this secret to forge new JWTs: '" << cracker.getFoundSecret() << "'\n";
            return 0;
        }
        
        // Dictionary attack if wordlist provided
        if(program.is_used("--wordlist")){
            std::string wordlist = program.get<std::string>("--wordlist");
            if(run_dictionary_attack(cracker, wordlist)){
                std::cout << "\n[+] Use this secret to forge new JWTs: '" << cracker.getFoundSecret() << "'\n";
                return 0;
            }
        }
        
        // Brute force attack if charset provided
        if(!program.get<std::string>("bruteforce").empty()){
            std::string charset = program.get<std::string>("bruteforce");
            int min_len = program.get<int>("min-len");
            int max_len = program.get<int>("max-len");
            int threads = program.get<int>("threads");
            
            if(run_bruteforce_attack(cracker, charset, min_len, max_len, threads)){
                std::cout << "\n[+] Use this secret to forge new JWTs: '" << cracker.getFoundSecret() << "'\n";
                return 0;
            }
        }
        
        if(!cracker.isFound()){
             if(!program.is_used("--wordlist") && program.get<std::string>("bruteforce").empty() && !program.get<bool>("alg-none") && !program.get<bool>("info")){
                std::cout << "\n[!] No attack method specified, and no info/alg-none check requested.\n";
                std::cout << "[!] Use --wordlist, --bruteforce, --alg-none, or --info.\n";
                std::cout << "[!] Example: ./jawarat <token> -w rockyou.txt\n";
                std::cout << "[!] Example: ./jawarat <token> -b \"abcdefghijklmnopqrstuvwxyz\" --max-len 8\n";
            }
            else {
                std::cout << "\n[-] All specified attacks completed. Secret not found.\n";
            }
        }
        
    }
    catch (const std::invalid_argument& e){
        std::cerr << "Initialization Error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e){
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
