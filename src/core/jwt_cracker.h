#ifndef JWT_CRACKER_H
#define JWT_CRACKER_H

#include <string>
#include <vector>
#include <atomic>
#include <mutex>
#include "jwt-cpp/jwt.h"

class JWTCracker {
private:
    std::string target_jwt_token;
    std::string jwt_header_part;
    std::string jwt_payload_part;
    std::string jwt_signature_part;
    std::atomic<bool> secret_found_flag{false};
    std::string found_secret_value;
    std::mutex output_access_mutex;
    
    std::vector<std::string> predefined_common_secrets = {
        "secret", "password", "123456", "admin", "root", "test",
        "key", "jwt", "token", "auth", "login", "user", "pass",
        "default", "demo", "example", "sample", "weak", "simple",
        "qwerty", "abc123", "password123", "secret123", "admin123", "1234", "123", "111", "12345",
        "letmein", "welcome", "iloveyou", "trustno1", "monkey", "dragon", "sunshine", "football",
        "baseball", "qwertyuiop", "12345678", "123456789", "1234567", "1234567890", "password1",
        "123123", "1q2w3e4r", "1qaz2wsx", "qazwsx", "zxcvbnm", "asdfghjkl", "qwerty123", "qwerty!@#"
    };

    void parseJWT();

public:
    JWTCracker(const std::string& jwt);
    
    bool testSecret(const std::string& secret);
    
    bool isFound() const;
    std::string getFoundSecret() const;
    void setFound(const std::string& secret);
    
    std::mutex& getOutputMutex();
    const std::string& getTargetJWT() const;
    const std::vector<std::string>& getCommonSecrets() const;

    // Accessors for parsed parts if needed by other modules, though typically not directly
    const std::string& getHeaderPart() const { return jwt_header_part; }
    const std::string& getPayloadPart() const { return jwt_payload_part; }
    const std::string& getSignaturePart() const { return jwt_signature_part; }
};

#endif // JWT_CRACKER_H
