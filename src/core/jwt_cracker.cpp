#include "core/jwt_cracker.h" // Updated path
#include <stdexcept>
#include <iostream> // For potential debug/error output, though typically handled by callers

JWTCracker::JWTCracker(const std::string& jwt) : target_jwt_token(jwt){
    parseJWT();
}

void JWTCracker::parseJWT(){
    size_t first_dot = target_jwt_token.find('.');
    size_t second_dot = target_jwt_token.find('.', first_dot + 1);
    
    if(first_dot == std::string::npos || second_dot == std::string::npos){
        throw std::invalid_argument("Invalid JWT format");
    }
    
    jwt_header_part = target_jwt_token.substr(0, first_dot);
    jwt_payload_part = target_jwt_token.substr(first_dot + 1, second_dot - first_dot - 1);
    jwt_signature_part = target_jwt_token.substr(second_dot + 1);
}

bool JWTCracker::testSecret(const std::string& secret){
    if(secret_found_flag.load()){ // Optimization: if already found by another thread
        return false; 
    }
    try{
        auto decoded = jwt::decode(target_jwt_token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{secret});
        verifier.verify(decoded);
        return true;
    } 
    catch(const jwt::error::signature_verification_exception& e){
        // This is expected for wrong secrets
        return false;
    } 
    catch(const std::exception& e){
        // Other errors (e.g., token format, though parseJWT should catch some)
        // Optionally log this, but for cracking, we mostly care about signature verification
        // std::cerr << "Error during testSecret with secret '" << secret << "': " << e.what() << std::endl;
        return false;
    }
}

bool JWTCracker::isFound() const{
    return secret_found_flag.load();
}

std::string JWTCracker::getFoundSecret() const{
    return found_secret_value;
}

void JWTCracker::setFound(const std::string& secret){
    secret_found_flag.store(true);
    found_secret_value = secret;
}

std::mutex& JWTCracker::getOutputMutex(){
    return output_access_mutex;
}

const std::string& JWTCracker::getTargetJWT() const{
    return target_jwt_token;
}

const std::vector<std::string>& JWTCracker::getCommonSecrets() const{
    return predefined_common_secrets;
}
