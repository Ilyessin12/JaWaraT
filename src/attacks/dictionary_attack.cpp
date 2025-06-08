#include "attacks/dictionary_attack.h" // Updated path
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip> // For std::flush

bool run_dictionary_attack(JWTCracker& cracker, const std::string& wordlist_path) {
    std::cout << "[*] Starting dictionary attack with: " << wordlist_path << "\n";
    
    std::ifstream file(wordlist_path);
    if (!file.is_open()) {
        std::cerr << "[-] Could not open wordlist file: " << wordlist_path << "\n";
        return false;
    }
    
    std::string word;
    size_t attempts = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    while (std::getline(file, word) && !cracker.isFound()) {
        // Remove trailing whitespace (common in wordlists)
        size_t last_char = word.find_last_not_of(" \t\r\n");
        if (std::string::npos != last_char) {
            word.erase(last_char + 1);
        } else { // Handle empty or all-whitespace lines
            word.clear(); 
        }

        if (word.empty()) continue; // Skip empty lines
        
        if (cracker.testSecret(word)) {
            cracker.setFound(word);
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            
            std::lock_guard<std::mutex> lock(cracker.getOutputMutex());
            std::cout << "\n[+] SECRET FOUND (dictionary): '" << word << "'\n";
            std::cout << "[+] Time taken: " << duration.count() << " ms\n";
            std::cout << "[+] Attempts: " << attempts + 1 << "\n";
            return true;
        }
        
        attempts++;
        if (attempts % 1000 == 0) {
            std::lock_guard<std::mutex> lock(cracker.getOutputMutex());
            std::cout << "\r[*] Tested " << attempts << " passwords from dictionary..." << std::flush;
        }
    }
    
    std::cout << "\n[-] Dictionary attack completed. Secret not found.\n";
    return false;
}