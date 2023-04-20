#include <aes.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <string>

#if OPENSSL_VERSION_NUMBER < 0x10100000
#define EVP_MD_CTX_new() EVP_MD_CTX_create()
#define EVP_MD_CTX_free(ctx) EVP_MD_CTX_destroy(ctx)
#define EVP_CIPHER_get_block_size EVP_CIPHER_block_size
#endif

std::string EncodeToString(const unsigned char *in, int inl) {
    std::string out_str{};
    auto block_size = inl * 4 / 3 + 3;
    auto out = (char *) malloc(block_size);
    if (0 != EVP_EncodeBlock(reinterpret_cast<unsigned char *>(out), in, inl)) {
        out_str = out;
    }
    if (nullptr != out) {
        free(out);
    }
    return out_str;
}
//
std::string EncodeToString(const std::string &in) {
    if (in.empty()) {
        return std::string{};
    }
    std::string out_str{};
    auto inl = (int) in.length();
    auto block_size = inl * 4 / 3 + 3;
    auto out = (char *) malloc(inl * block_size);
    if (0 != EVP_EncodeBlock(reinterpret_cast<unsigned char *>(out), reinterpret_cast<const unsigned char *>(in.c_str()), inl)) {
        out_str = out;
    }
    if (nullptr != out) {
        free(out);
    }
    return out_str;
}

//
std::string DecodeString(const std::string &in) {
    std::string out_str{};
    auto inl = (int) in.length();
    auto out = (char *) malloc(inl * 3 / 4);

    if (EVP_DecodeBlock(reinterpret_cast<unsigned char *>(out), reinterpret_cast<const unsigned char *>(in.c_str()), inl) > 0) {
        out_str = out;
    }

    if (nullptr != out) {
        free(out);
    }
    return out_str;
}

bool Encrypt(const std::string &content, const std::string &passphrase, std::string &cipher_content) {
    auto name = "aes-256-cbc";

    EVP_get_digestbynid(NID_aes_256_cbc);
    auto cipher = EVP_get_cipherbyname(name);

    if (cipher == nullptr) {
        return false;
    }

    unsigned char key[32]{};
    unsigned char iv[16]{};

    unsigned char salt[PKCS5_SALT_LEN] = {};

    RAND_bytes(reinterpret_cast<unsigned char *>(salt), sizeof(salt));
    const auto &ps = passphrase;
    EVP_BytesToKey(cipher, EVP_md5(), (const unsigned char *) salt, reinterpret_cast<const unsigned char *>(ps.c_str()), (int) ps.length(), 1, key, iv);

    auto ctx = EVP_CIPHER_CTX_new();
    if (nullptr == ctx) {
        return false;
    }

    if (0 == EVP_EncryptInit(ctx, cipher, key, iv)) {
        return false;
    }

    auto block_size = EVP_CIPHER_get_block_size(cipher);

    const auto &in = content;
    auto inl = in.length();

    auto *out = (unsigned char *) malloc(inl + block_size);
    memset(out, 0, inl * 2);
    int total = {0};
    int outl = {0};
    if (0 == EVP_EncryptUpdate(ctx, out, &outl, (const unsigned char *) in.c_str(), (int) inl)) {
        return false;
    }
    total += outl;
    if (0 == EVP_EncryptFinal(ctx, out + total, &outl)) {
        return false;
    }
    total += outl;
    // magic = "Salted__";
    std::string cipher_out_byte = "Salted__";
    // salt
    cipher_out_byte.append(reinterpret_cast<const char *>(salt), sizeof(salt));
    // cipher
    cipher_out_byte.append(reinterpret_cast<const char *>(out), total);

    cipher_content = EncodeToString(cipher_out_byte);
    if (nullptr != out) {
        free(out);
    }

    EVP_CIPHER_CTX_free(ctx);

    return true;
}

bool Decrypt(const std::string &cipher_content, const std::string &passphrase, std::string &content) {
    auto name = "aes-256-cbc";
    char err_buf[1024] = {};
    printf("1 :%s\n", ERR_error_string(ERR_get_error(), err_buf));
    auto cipher = EVP_get_cipherbyname(name);
    if (cipher == nullptr) {
        return false;
    }
    auto ctx = EVP_CIPHER_CTX_new();
    if (nullptr == ctx) {
        return false;
    }

    unsigned char key[32]{};
    unsigned char iv[16]{};

    auto bd = DecodeString(cipher_content);
    unsigned char salt[PKCS5_SALT_LEN] = {};
    memcpy(salt, bd.c_str() + 8, 8);
    const auto &ps = passphrase;
    EVP_BytesToKey(cipher, EVP_md5(), (const unsigned char *) salt, (const unsigned char *) ps.c_str(), (int) ps.length(), 1, key, iv);
    if (0 == EVP_DecryptInit(ctx, cipher, key, iv)) {
        return false;
    }
    auto inl = (int) (bd.length() - 16);
    int total = {0};
    int outl = {0};
    auto block_size = EVP_CIPHER_get_block_size(cipher);
    auto *out = (unsigned char *) malloc(inl + block_size);
    memset(out, 0, outl * 2);
    EVP_DecryptUpdate(ctx, out, &outl, reinterpret_cast<const unsigned char *>(bd.c_str() + 16), inl);
    total += outl;
    EVP_DecryptFinal(ctx, out + total, &outl);
    total += outl;
    std::string res{reinterpret_cast<const char *>(out), static_cast<std::size_t>(total)};
    content = std::move(res);

    free(out);

    return true;
}

std::string Signature(const std::string &msg, const std::string &priv) {

    std::shared_ptr<evp_md_ctx_st> sh_ctx{EVP_MD_CTX_new(), [](evp_md_ctx_st *ctx) {
                                              if (nullptr != ctx) {
                                                  EVP_MD_CTX_free(ctx);
                                              }
                                          }};
    if (nullptr == sh_ctx) {
        return std::string{};
    }

    auto md = EVP_get_digestbyname("sha256");
    if (!EVP_DigestInit(sh_ctx.get(), md)) {
        return std::string{};
    }

    auto key = EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, nullptr, reinterpret_cast<const unsigned char *>(priv.c_str()), priv.length());
    std::shared_ptr<EVP_PKEY> sh_key{key, [](EVP_PKEY *key) {
                                         if (nullptr != key) {
                                             EVP_PKEY_free(key);
                                         }
                                     }};

    if (1 != EVP_DigestSignInit(sh_ctx.get(), nullptr, md, nullptr, sh_key.get())) {
        return std::string{};
    }

    if (1 != EVP_DigestSignUpdate(sh_ctx.get(), msg.c_str(), msg.length())) {
        return std::string{};
    }

    unsigned char md_value[EVP_MAX_MD_SIZE];
    std::size_t md_len;
    if (1 != EVP_DigestSignFinal(sh_ctx.get(), md_value, &md_len)) {
        return std::string{};
    }

    return EncodeToString(md_value, (int) md_len);
}
#define EVP_get_digestbyname(name) EVP_##name()
int main() {

    auto passphrase = "S!C@U#T$E%C^H&8620";
    std::string content = "U2FsdGVkX1+w7saTs/gNuxcm7YkK5+r2aBI0CBih0+Q=";
    std::string cipher_content{};
//
//    Encrypt(content, passphrase, cipher_content);
//    std::cout << cipher_content << std::endl;
//
//    content.clear();
    cipher_content = "U2FsdGVkX1+w7saTs/gNuxcm7YkK5+r2aBI0CBih0+Q=";
    if(!Decrypt(cipher_content, passphrase, content)) {
        std::cout << "err" << std::endl;
    }
    std::cout << content << std::endl;

    return 0;
}