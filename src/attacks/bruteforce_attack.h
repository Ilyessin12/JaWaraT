#ifndef BRUTEFORCE_ATTACK_H
#define BRUTEFORCE_ATTACK_H

#include <string>
#include "core/jwt_cracker.h" // Updated path: For JWTCracker reference

bool run_bruteforce_attack(JWTCracker& cracker, const std::string& charset, int min_len, int max_len, int threads);

#endif // BRUTEFORCE_ATTACK_H
