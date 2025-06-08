#include "attacks/bruteforce_attack.h" // Updated path
#include <vector>
#include <thread>
#include <iostream>
#include <chrono>
#include <iomanip> // For std::flush
#include <algorithm> // For std::pow if needed, though direct multiplication is used

// Helper function for generating combinations for a specific thread
void generate_combinations_for_thread(JWTCracker& cracker, const std::string& charset, int length,
                                      long long start_idx, long long end_idx, int thread_id){
    std::string combination(length, ' ');
    std::vector<int> indices(length, 0);
    long long current_num = start_idx;

    // Initialize indices to match start_idx
    long long temp_start_idx = start_idx;
    for(int i = length - 1; i >= 0; --i){
        indices[i] = temp_start_idx % charset.length();
        temp_start_idx /= charset.length();
    }

    while(current_num < end_idx && !cracker.isFound()){
        // Build current combination string from indices
        for(int i = 0; i < length; ++i){
            combination[i] = charset[indices[i]];
        }

        if(cracker.testSecret(combination)){
            cracker.setFound(combination);
            std::lock_guard<std::mutex> lock(cracker.getOutputMutex());
            std::cout << "\n[+] SECRET FOUND (bruteforce): '" << combination << "'\n";
            std::cout << "[+] Thread " << thread_id << " found the secret!\n";
            return;
        }

        // Increment indices to get the next combination (like incrementing a base-N number)
        int k = length - 1;
        while(k >= 0){
            indices[k]++;
            if(indices[k] < charset.length()) break; // No carry
            indices[k] = 0; // Carry
            k--;
        }
        
        current_num++;

        if(current_num % 50000 == 0 && thread_id == 0){ // Print progress from one thread
            std::lock_guard<std::mutex> lock(cracker.getOutputMutex());
            std::cout << "\r[*] Bruteforce (len " << length << ", thread " << thread_id << ") tested "
                      << current_num - start_idx << " combinations..." << std::flush;
        }
    }
}

// Main worker function for each thread in bruteforce
void bruteforce_worker_thread(JWTCracker& cracker, const std::string charset, 
                              int min_len, int max_len, int thread_id, int total_threads){
    for(int len = min_len; len <= max_len && !cracker.isFound(); ++len){
        long long total_combinations_for_len = 1;
        for(int i=0; i<len; ++i){ // Calculate charset.length() ^ len safely
            if(__builtin_mul_overflow(total_combinations_for_len, charset.length(), &total_combinations_for_len)){
                 total_combinations_for_len = -1; // Indicate overflow / too large
                 break;
            }
        }
        if(total_combinations_for_len == 0 && len > 0) total_combinations_for_len = -1; // charset empty or len=0 case
        if(total_combinations_for_len == -1){ // Too many combinations, likely impractical
            if(thread_id == 0){
                 std::lock_guard<std::mutex> lock(cracker.getOutputMutex());
                 std::cerr << "\nWarning: Too many combinations for length " << len << ". Skipping or may take extremely long." << std::endl;
            }
            continue; // Or break, depending on desired behavior for huge keyspaces
        }


        long long combinations_per_thread = total_combinations_for_len / total_threads;
        long long remainder_combinations = total_combinations_for_len % total_threads;

        long long start_idx = thread_id * combinations_per_thread + std::min((long long)thread_id, remainder_combinations);
        long long end_idx = start_idx + combinations_per_thread + (thread_id < remainder_combinations ? 1 : 0);
        
        if(thread_id == 0 && len == min_len){ // Only print once per length from main thread
             std::lock_guard<std::mutex> lock(cracker.getOutputMutex());
             std::cout << "\r[*] Starting bruteforce for length " << len << " (Total: " << total_combinations_for_len << ")" << std::endl;
        }


        generate_combinations_for_thread(cracker, charset, len, start_idx, end_idx, thread_id);
        if(cracker.isFound()) return;
    }
}


bool run_bruteforce_attack(JWTCracker& cracker, const std::string& charset, 
                           int min_len, int max_len, int threads){
    if(charset.empty()){
        std::cerr << "[-] Character set for bruteforce cannot be empty.\n";
        return false;
    }
    std::cout << "[*] Starting brute force attack...\n";
    std::cout << "[*] Character set: " << charset << "\n";
    std::cout << "[*] Length range: " << min_len << "-" << max_len << "\n";
    std::cout << "[*] Threads: " << threads << "\n";
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> thread_pool;
    for(int i = 0; i < threads; ++i){
        thread_pool.emplace_back(bruteforce_worker_thread, std::ref(cracker), 
                                 charset, min_len, max_len, i, threads);
    }
    
    for(auto& t : thread_pool){
        if(t.joinable()){
            t.join();
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
    
    std::lock_guard<std::mutex> lock(cracker.getOutputMutex()); // Ensure final messages are clean
    if(cracker.isFound()){
        std::cout << "\n[+] Brute force attack finished. Secret found.\n";
        std::cout << "[+] Time taken: " << duration.count() << " seconds\n";
        return true;
    }
    else {
        std::cout << "\n[-] Brute force attack completed. Secret not found.\n";
        std::cout << "[*] Time taken: " << duration.count() << " seconds\n";
        return false;
    }
}