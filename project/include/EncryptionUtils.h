#pragma once
#include <iomanip>
#include <vector>
#include <sodium.h>

// P1
class EncryptionUtils
{
public:
  EncryptionUtils() = default;

  // AN (24/07/2024): implementations of functions below are little bit changed defaults from Sodium lib docs
  // https://libsodium.gitbook.io/doc/hashing/generic_hashing
  // here i used some reference material from stackoverflow and chatgpt too
  static auto hash_string(const std::string& input) -> std::string;

  static auto string_to_key(const std::string& key_str) -> std::vector<unsigned char>;

  static auto encrypt_aes(const std::string& plain, const std::string& key_str) -> std::string;

  static auto decrypt_aes(const std::string& cipher, const std::string& key_str) -> std::string;
};