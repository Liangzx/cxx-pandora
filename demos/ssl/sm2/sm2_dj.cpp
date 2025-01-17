#include "scrt/pkey_codec.h"

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/obj_mac.h>
#include <openssl/pem.h>
#include <ostream>
#include <string>
#include <vector>

// 将字节数组转换为十六进制字符串
std::string byte_to_hex(const scrt::ustring& bytes)
{
    std::ostringstream oss;
    for (unsigned char byte : bytes) {
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(byte);
    }
    return oss.str();
}

int get_der_public_key_from_hex(std::string& der_pub)
{
    // 公钥的十六进制字符串
    std::string pub_hex =
        "04711efc69cb48c8d72531fe7a5294b78fd80a8f8f4c8a93f9c5f3e8a4406904ecab0d"
        "f5242bc4c50d9bc1f96495241953f1dcb47f2630c216996ba07e672aba65";

    // 初始化椭圆曲线
    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_sm2);
    if (group == nullptr) {
        std::cerr << "Failed to create EC_GROUP" << std::endl;
        return -1;
    }

    // 将十六进制字符串转换为EC_POINT
    EC_POINT* pub_key =
        EC_POINT_hex2point(group, pub_hex.c_str(), nullptr, nullptr);
    if (pub_key == nullptr) {
        std::cerr << "Failed to convert hex to EC_POINT" << std::endl;
        EC_GROUP_free(group);
        return -1;
    }

    // 生成SM2密钥对
    EC_KEY* ec_key = EC_KEY_new();
    if (ec_key == nullptr) {
        std::cerr << "Failed to create EC_KEY" << std::endl;
        EC_POINT_free(pub_key);
        EC_GROUP_free(group);
        return -1;
    }

    if (EC_KEY_set_group(ec_key, group) != 1) {
        std::cerr << "Failed to set EC_GROUP" << std::endl;
        EC_KEY_free(ec_key);
        EC_POINT_free(pub_key);
        EC_GROUP_free(group);
        return -1;
    }

    // 设置公钥
    if (EC_KEY_set_public_key(ec_key, pub_key) != 1) {
        std::cerr << "Failed to set public key" << std::endl;
        EC_KEY_free(ec_key);
        EC_POINT_free(pub_key);
        EC_GROUP_free(group);
        return -1;
    }

    // 将EC_KEY转换为EVP_PKEY
    EVP_PKEY* pkey = EVP_PKEY_new();
    if (pkey == nullptr) {
        std::cerr << "Failed to create EVP_PKEY" << std::endl;
        EC_KEY_free(ec_key);
        EC_POINT_free(pub_key);
        EC_GROUP_free(group);
        return -1;
    }

    if (EVP_PKEY_set1_EC_KEY(pkey, ec_key) != 1) {
        std::cerr << "Failed to set EC_KEY" << std::endl;
        EVP_PKEY_free(pkey);
        EC_KEY_free(ec_key);
        EC_POINT_free(pub_key);
        EC_GROUP_free(group);
        return -1;
    }

    // 将公钥转换为DER编码的字节数组并写入到Bio对象中
    BIO* bio = BIO_new(BIO_s_mem());
    if (bio == nullptr) {
        std::cerr << "Failed to create BIO" << std::endl;
        EVP_PKEY_free(pkey);
        EC_KEY_free(ec_key);
        EC_POINT_free(pub_key);
        EC_GROUP_free(group);
        return -1;
    }

    int der_len = i2d_PUBKEY_bio(bio, pkey);
    if (der_len <= 0) {
        std::cerr << "Failed to convert EVP_PKEY to DER" << std::endl;
        BIO_free(bio);
        EVP_PKEY_free(pkey);
        EC_KEY_free(ec_key);
        EC_POINT_free(pub_key);
        EC_GROUP_free(group);
        return -1;
    }

    std::cout << "DER encoded public key der_len: " << der_len << std::endl;
    // 打印DER编码的字节数组
    char* der = new char[der_len + 1];
    BIO_read(bio, der, der_len);
    der[der_len] = '\0';
    std::cout << "DER encoded public key: " << der << std::endl;

    der_pub = std::string(der, der_len);
    // 释放内存
    delete[] der;
    BIO_free(bio);
    EVP_PKEY_free(pkey);
    EC_POINT_free(pub_key);
    EC_KEY_free(ec_key);
    EC_GROUP_free(group);
    return 1;
}

int func1()
{
    std::string msg = "123456";
    std::string der_pub;

    get_der_public_key_from_hex(der_pub);
    PKEY_Encoder encoder(ENCRYPT_SM2,
                         (const ACE_Byte*)msg.c_str(),
                         msg.size(),
                         (const ACE_Byte*)der_pub.c_str(),
                         der_pub.size(),
                         false);

    scrt::ustring encoded(encoder.get(), encoder.length());
    std::cout << "---------------------------" << std::endl;
    std::cout << byte_to_hex(encoded) << std::endl;

    return 0;
}

int main()
{
    func1();
    return 0;
}
