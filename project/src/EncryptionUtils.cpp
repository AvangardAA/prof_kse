#include "EncryptionUtils.h"

// AN (24/07/2024): for implementation remark see my comment in .h file

auto EncryptionUtils::hash_string(const std::string& input) -> std::string
{
    std::vector<unsigned char> hash(crypto_generichash_BYTES);
    crypto_generichash(hash.data(), hash.size(),
                    reinterpret_cast<const unsigned char*>(input.data()), input.size(),
                    nullptr, 0);

    std::stringstream ss;
    for (unsigned char byte : hash) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    
    return ss.str();
}

auto EncryptionUtils::string_to_key(const std::string& key_str) -> std::vector<unsigned char> 
{
    std::vector<unsigned char> key;
    key.reserve(key_str.size() / 2);
    
    for (size_t i = 0; i < key_str.size(); i += 2) 
    {
        std::string byte_str = key_str.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(std::stoi(byte_str, nullptr, 16));
        key.push_back(byte);
    }

    if (key.size() != crypto_secretbox_KEYBYTES) 
    {
        throw std::runtime_error("Invalid key size");
    }
    
    return key;
}

auto EncryptionUtils::encrypt_aes(const std::string& plain, const std::string& key_str) -> std::string 
{
    std::vector<unsigned char> key = string_to_key(key_str);

    std::vector<unsigned char> ciphertext(crypto_secretbox_MACBYTES + plain.size());

    std::vector<unsigned char> nonce(crypto_secretbox_NONCEBYTES);
    randombytes_buf(nonce.data(), nonce.size());

    if (crypto_secretbox_easy(ciphertext.data(), reinterpret_cast<const unsigned char*>(plain.c_str()), plain.size(),
                            nonce.data(), key.data()) != 0) {
        throw std::runtime_error("Encryption failed");
    }

    ciphertext.insert(ciphertext.begin(), nonce.begin(), nonce.end());

    return std::string(reinterpret_cast<char*>(ciphertext.data()), ciphertext.size());
}

auto EncryptionUtils::decrypt_aes(const std::string& cipher, const std::string& key_str) -> std::string 
{
    std::vector<unsigned char> ciphertext(cipher.begin(), cipher.end());
    std::vector<unsigned char> key = string_to_key(key_str);

    if (ciphertext.size() < crypto_secretbox_MACBYTES + crypto_secretbox_NONCEBYTES) {
        throw std::runtime_error("Ciphertext is too short");
    }

    std::vector<unsigned char> nonce(ciphertext.begin(), ciphertext.begin() + crypto_secretbox_NONCEBYTES);

    std::vector<unsigned char> decrypted(ciphertext.size() - crypto_secretbox_NONCEBYTES);
    if (crypto_secretbox_open_easy(decrypted.data(), ciphertext.data() + crypto_secretbox_NONCEBYTES, ciphertext.size() - crypto_secretbox_NONCEBYTES,
                                    nonce.data(), key.data()) != 0) {
        throw std::runtime_error("Decryption failed");
    }

    return std::string(reinterpret_cast<char*>(decrypted.data()), decrypted.size());
}