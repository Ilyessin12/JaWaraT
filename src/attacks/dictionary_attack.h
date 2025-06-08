#ifndef DICTIONARY_ATTACK_H
#define DICTIONARY_ATTACK_H

#include <string>
#include "core/jwt_cracker.h" // Updated path: For JWTCracker reference

bool run_dictionary_attack(JWTCracker& cracker, const std::string& wordlist_path);

#endif // DICTIONARY_ATTACK_H
