#ifndef ATTACK_UTILS_H
#define ATTACK_UTILS_H

#include <string>
#include "core/jwt_cracker.h" // Updated path: For JWTCracker reference

void check_alg_none_vulnerability(const std::string& target_jwt);
bool check_common_secrets_attack(JWTCracker& cracker);

#endif // ATTACK_UTILS_H
