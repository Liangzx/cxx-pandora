/*
 *  Copyright 2014-2024 The GmSSL Project. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the License); you may
 *  not use this file except in compliance with the License.
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 */

#include <cstdio>
#include <gmssl/asn1.h>
#include <gmssl/endian.h>
#include <gmssl/error.h>
#include <gmssl/mem.h>
#include <gmssl/sm2.h>
#include <gmssl/sm3.h>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

// 将字节数组转换为十六进制字符串
std::string byte_to_hex(const std::string& bytes)
{
    std::ostringstream oss;
    for (unsigned char byte : bytes) {
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(byte);
    }
    return oss.str();
}

std::string hex_to_bytes(const std::string& hex)
{
    std::string bytes;
    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        unsigned char byte =
            (unsigned char)strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    return bytes;
}

static int test_sm2_encrypt_with_pubkey(void)
{
    SM2_KEY sm2_key;
    std::string plaintext = "123456";

    uint8_t plainbuf[SM2_MAX_PLAINTEXT_SIZE] = {0};
    size_t plainlen = 0;
    int r = 0;
    unsigned char cipher_text[1024] = {0};
    int cipher_len = 0;

    size_t i = 0;

    // if (sm2_key_generate(&sm2_key) != 1) {

    //     return -1;
    // }

    FILE* fp_private = fopen("private_key.pem", "r");
    FILE* fp_public = fopen("public_key.pem", "r");
    // sm2_public_key_info_to_pem(&sm2_key, fp_public);
    // sm2_private_key_to_pem(&sm2_key, fp_private);

    sm2_private_key_from_pem(&sm2_key, fp_private);
    sm2_public_key_info_from_pem(&sm2_key, fp_public);

    if (sm2_encrypt(&sm2_key,
                    (const uint8_t*)plaintext.c_str(),
                    plaintext.size(),
                    cipher_text,
                    (size_t*)&cipher_len) != 1) {

        return -1;
    }

    std::cout << byte_to_hex(std::string((char*)cipher_text, cipher_len))
              << std::endl;

    unsigned char clear_text[1024] = {0};
    int clear_len = 0;
    if (sm2_decrypt(&sm2_key,
                    cipher_text,
                    cipher_len,
                    clear_text,
                    (size_t*)&clear_len) != 1) {
        return -1;
    }

    std::cout << std::string((char*)clear_text, clear_len) << std::endl;

    printf("%s() ok\n", __FUNCTION__);
    fclose(fp_private);
    fclose(fp_public);
    return 1;
}

int test_sm2_en2()
{
    SM2_KEY sm2_key;
    std::string plaintext = "123456";

    uint8_t plainbuf[SM2_MAX_PLAINTEXT_SIZE] = {0};
    size_t plainlen = 0;
    int r = 0;
    unsigned char out[1024] = {0};
    int outlen = 0;

    size_t i = 0;

    if (sm2_key_generate(&sm2_key) != 1) {
        return -1;
    }

    std::string x =
        "6fc6048a394aa4eadc45011b270cfa93b2f3cb5302b4897581e411276d54c501";
    std::string y =
        "75755a03227e96e66da79322cbf724dccc9fba29fd347527495650286f3f00b6";

    std::string pri =
        "7c072b31c7a60d17ffba357aaa00ef01006355b06ed6d6d01a63a6ce75704bfb";

    memcpy(&sm2_key.public_key.X, byte_to_hex(x).c_str(), 32);
    memcpy(&sm2_key.public_key.Y, byte_to_hex(y).c_str(), 32);
    memcpy(&sm2_key.private_key, byte_to_hex(y).c_str(), 32);

    SM2_CIPHERTEXT sm_cipher_ctx;
    if (sm2_do_encrypt(&sm2_key,
                       (const uint8_t*)plaintext.c_str(),
                       plaintext.size(),
                       &sm_cipher_ctx) != 1) {
        return -1;
    }
    std::cout << (int)sm_cipher_ctx.ciphertext_size << std::endl;

    unsigned char raw_out[1024] = {0};
    // C1 公钥
    memcpy(raw_out, &sm_cipher_ctx, 64);
    // C2 密文
    memcpy(raw_out + 64, &sm_cipher_ctx.ciphertext, 6);
    // C3 hash 摘要
    memcpy(raw_out + 70, &sm_cipher_ctx.hash, 32);
    std::string cipher_text = std::string((char*)raw_out, 102);
    std::cout << byte_to_hex(cipher_text) << std::endl;
    printf("%s() ok\n", __FUNCTION__);

    uint8_t der_text[1024];
    uint8_t* der_text_p = der_text;
    size_t der_len = 0;
    if (sm2_ciphertext_to_der(&sm_cipher_ctx, &der_text_p, &der_len) != 1) {
        return -1;
    }

    if (sm2_decrypt(&sm2_key, der_text_p, der_len, out, (size_t*)&outlen) !=
        1) {
        return -1;
    }
    std::cout << std::string((char*)out, outlen) << std::endl;

    return 1;
}

int main()
{
    test_sm2_encrypt_with_pubkey();
    return 0;
}
