#pragma once 
#include <sodium.h>
#include <string>
#include <stdexcept>
inline void init(){
    if(sodium_init()<0){
        throw std::runtime_error("Lib failed to init");
    }
}
inline std::string hash(const std::string& pass){
    char out[crypto_pwhash_STRBYTES];
    if(crypto_pwhash_str(out,pass.c_str(),pass.size(),crypto_pwhash_OPSLIMIT_INTERACTIVE,crypto_pwhash_MEMLIMIT_INTERACTIVE)!=0){
        throw std::runtime_error("Password cant hash");

    }
    return std::string(out);
}
inline bool verifyHash(const std::string& stored, const std::string& pass){
    return crypto_pwhash_str_verify(stored.c_str(),pass.c_str(),pass.size())==0;
}