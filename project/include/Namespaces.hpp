#pragma once
#include <map>
#include <string>

namespace Messages
{
    constexpr std::string_view missing_credentials(R"({"status":"error","msg":"missing credentials"})");
    constexpr std::string_view already_existing(R"({"status":"error","msg":"already existing user"})");
    constexpr std::string_view successfully_registered(R"({"status":"ok","msg":"registered succesfully"})");
    constexpr std::string_view not_exist(R"({"status":"error","msg":"user dont exist"})");
    constexpr std::string_view auth_failed(R"({"status":"error","msg":"wrong password"})");
    constexpr std::string_view auth_success(R"({"status":"ok","msg":"auth succesfully"})");
    constexpr std::string_view not_authorized(R"({"status":"error","msg":"not authorized"})");
    constexpr std::string_view wrong_order_params(R"({"status":"error","msg":"wrong order params"})");
    constexpr std::string_view order_submit_success(R"({"status":"ok","msg":"order submitted succesfully"})");
    constexpr std::string_view wrong_LOB_params(R"({"status":"error","msg":"wrong LOB params"})");
}

namespace CustomTypes
{
    using login_encrypted = std::map<std::string, std::string>;
}