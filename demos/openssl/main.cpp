#include <iostream>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>


static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

// base64 编码
std::string base64_encode(char const *bytes_to_encode, int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}

// base64 解码
std::string base64_decode(std::string &encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

std::string aes_256_cbc_encode(const std::string& password, const std::string& data)
{
    // 这里默认将iv全置为字符0
    unsigned char iv[AES_BLOCK_SIZE] = { '0','0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };

    AES_KEY aes_key;
    if (AES_set_encrypt_key((const unsigned char*)password.c_str(), password.length() * 8, &aes_key) < 0)
    {
        //assert(false);
        return "";
    }
    std::string strRet;
    std::string data_bak = data;
    unsigned int data_length = data_bak.length();

    // ZeroPadding
    int padding = 0;
    if (data_bak.length() % (AES_BLOCK_SIZE) > 0)
    {
        padding = AES_BLOCK_SIZE - data_bak.length() % (AES_BLOCK_SIZE);
    }
    // 在一些软件实现中，即使是16的倍数也进行了16长度的补齐
    /*else
	{
		padding = AES_BLOCK_SIZE;
	}*/

    data_length += padding;
    while (padding > 0)
    {
        data_bak += '\0';
        padding--;
    }

    for (unsigned int i = 0; i < data_length / (AES_BLOCK_SIZE); i++)
    {
        std::string str16 = data_bak.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        unsigned char out[AES_BLOCK_SIZE];
        ::memset(out, 0, AES_BLOCK_SIZE);
        AES_cbc_encrypt((const unsigned char*)str16.c_str(), out, AES_BLOCK_SIZE, &aes_key, iv, AES_ENCRYPT);
        strRet += std::string((const char*)out, AES_BLOCK_SIZE);
    }
    return strRet;
}

std::string aes_256_cbc_decode(const std::string& password, const std::string& strData)
{
    // 这里默认将iv全置为字符0
    unsigned char iv[AES_BLOCK_SIZE] = { '0','0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };

    AES_KEY aes_key;
    if (AES_set_decrypt_key((const unsigned char*)password.c_str(), password.length() * 8, &aes_key) < 0)
    {
        //assert(false);
        return "";
    }
    std::string strRet;
    for (unsigned int i = 0; i < strData.length() / AES_BLOCK_SIZE; i++)
    {
        std::string str16 = strData.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        unsigned char out[AES_BLOCK_SIZE];
        ::memset(out, 0, AES_BLOCK_SIZE);
        AES_cbc_encrypt((const unsigned char*)str16.c_str(), out, AES_BLOCK_SIZE, &aes_key, iv, AES_DECRYPT);
        strRet += std::string((const char*)out, AES_BLOCK_SIZE);
    }
    return strRet;
}

int main()
{
    // 原始字符串
    std::string str = "test342432535534654365476456456436545645000000000000000000000001";

    std::cout << "str(origin): " << str.c_str() << std::endl;

    // AES key
    std::string key = "S!C@U#T$E%C^H&86";

    std::string str_encode = aes_256_cbc_encode(key, str);
    std::string str_encode_base64 = base64_encode(str_encode.c_str(), str_encode.length());
    // 加密后的结果，以base64编码输出
    std::cout << "str_encode_base64: " << str_encode_base64.c_str() << std::endl;

    std::string str_decode_base64 = base64_decode(str_encode_base64);
    std::string str_decode = aes_256_cbc_decode(key, str_decode_base64);

    //解密后的结果
    std::cout << "str_decode: " << str_decode.c_str() << std::endl;

    return 0;
}
/**
 * echo -n 'U2FsdGVkX18Jva6EqAPSC2IS/7Wtk8//fR1UjPdqtio=' | openssl aes-256-cbc -k 'S!C@U#T$E%C^H&8620' -base64 -md md5
 * --U2FsdGVkX1/diA5RtcHvUzbyryhXFjlrPuXBKw/t8bw=
 * echo  'U2FsdGVkX197eDLMoczKz1qgQlI+oiSwH6L3gU7HSCE=' | openssl aes-256-cbc -d -k 'S!C@U#T$E%C^H&8620' -base64 -md md5
 */


/////////////-------- bingocloud crypto-------------
//#ifndef _BINGO_CRYPTO_H_
//#define _BINGO_CRYPTO_H_
//#include <cstring>
//#include <iostream>
//#include <memory>
//#include <openssl/evp.h>
//#include <openssl/rand.h>
//
//#include "bl_secure_string.h"
////
//#if OPENSSL_VERSION_NUMBER < 0x10100000
//#define EVP_MD_CTX_new() EVP_MD_CTX_create()
//#define EVP_MD_CTX_free(ctx) EVP_MD_CTX_destroy(ctx)
//#endif
//
//namespace crypto
//{
//// base64
//std::string EncodeToString(const unsigned char *in, int inl);
//std::string EncodeToString(const std::string &in);
//// hmac sha256
//bool Signature(const std::string &msg, const bl::secure_string &priv, std::string &sig);
//}
//#endif // _BINGO_CRYPTO_H_
// .cpp -----
//#include "bingo_crypto.h"
//
//namespace crypto
//{
//
//std::string EncodeToString(const unsigned char *in, int inl)
//{
//  std::string out_str{};
//  auto block_size = inl * 4 / 3 + 3;
//  auto out = (char *)malloc(block_size);
//  if (0 != EVP_EncodeBlock(reinterpret_cast<unsigned char *>(out), in, inl))
//  {
//    out_str = out;
//  }
//  if (nullptr != out)
//  {
//    free(out);
//  }
//  return out_str;
//}
//
//std::string EncodeToString(const std::string &in)
//{
//  if (in.empty())
//  {
//    return std::string{};
//  }
//  std::string out_str{};
//  auto inl = (int)in.length();
//  auto block_size = inl * 4 / 3 + 3;
//  auto out = (char *)malloc(inl * block_size);
//  if (0 != EVP_EncodeBlock(reinterpret_cast<unsigned char *>(out), reinterpret_cast<const unsigned char *>(in.c_str()), inl))
//  {
//    out_str = out;
//  }
//  if (nullptr != out)
//  {
//    free(out);
//  }
//  return out_str;
//}
//
//bool Signature(const std::string &msg, const bl::secure_string &priv, std::string & sig)
//{
//
//  std::shared_ptr<EVP_MD_CTX> sh_ctx{EVP_MD_CTX_new(), [](EVP_MD_CTX *ctx)
//                                     {
//                                       if (nullptr != ctx)
//                                       {
//                                         EVP_MD_CTX_free(ctx);
//                                       }
//                                     }};
//  if (nullptr == sh_ctx)
//  {
//    false;
//  }
//
//  auto md = EVP_sha256();
//  if (!EVP_DigestInit(sh_ctx.get(), md))
//  {
//    return false;
//  }
//
//  auto key = EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, nullptr, reinterpret_cast<const unsigned char *>(priv.c_str()), priv.length());
//  std::shared_ptr<EVP_PKEY> sh_key{key, [](EVP_PKEY *key)
//                                   {
//                                     if (nullptr != key)
//                                     {
//                                       EVP_PKEY_free(key);
//                                     }
//                                   }};
//
//  if (1 != EVP_DigestSignInit(sh_ctx.get(), nullptr, md, nullptr, sh_key.get()))
//  {
//    return false;
//  }
//
//  if (1 != EVP_DigestSignUpdate(sh_ctx.get(), msg.c_str(), msg.length()))
//  {
//    return false;
//  }
//
//  unsigned char md_value[EVP_MAX_MD_SIZE];
//  std::size_t md_len;
//  if (1 != EVP_DigestSignFinal(sh_ctx.get(), md_value, &md_len))
//  {
//    return false;
//  }
//
//  sig = EncodeToString(md_value, (int)md_len);
//  return !sig.empty();
//}
//}