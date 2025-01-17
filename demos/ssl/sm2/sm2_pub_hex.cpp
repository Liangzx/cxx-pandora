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

/* ========= 添加SM2密文解析的代码,这里是需要用到的头文件和宏定义部分,直接摘抄自sm2_crypt.c ========= */
#include <openssl/asn1.h>
#include <openssl/asn1t.h>

typedef struct SM2_Ciphertext_st SM2_Ciphertext;
DECLARE_ASN1_FUNCTIONS(SM2_Ciphertext)

struct SM2_Ciphertext_st
{
    BIGNUM *C1x;
    BIGNUM *C1y;
    ASN1_OCTET_STRING *C3;
    ASN1_OCTET_STRING *C2;
};

ASN1_SEQUENCE(SM2_Ciphertext) = {
    ASN1_SIMPLE(SM2_Ciphertext, C1x, BIGNUM),
    ASN1_SIMPLE(SM2_Ciphertext, C1y, BIGNUM),
    ASN1_SIMPLE(SM2_Ciphertext, C3, ASN1_OCTET_STRING),
    ASN1_SIMPLE(SM2_Ciphertext, C2, ASN1_OCTET_STRING),
} ASN1_SEQUENCE_END(SM2_Ciphertext)

IMPLEMENT_ASN1_FUNCTIONS(SM2_Ciphertext)
/* =========SM2密文解析功能头文件及宏定义引入部分结束========= */

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

int func1()
{
    // 公钥的十六进制字符串
    std::string pub_hex = "046fc6048a394aa4eadc45011b270cfa93b2f3cb5302b4897581e411276d54c50175755a03227e96e66da79322cbf724dccc9fba29fd347527495650286f3f00b6";

    /* 根据选择的椭圆曲线生成密钥参数 group */
    auto group = EC_GROUP_new_by_curve_name(NID_sm2);

    if (group == NULL) {
        printf("EC_GROUP_new_by_curve_name err!\n");
        return -1;
    }

    /* 构造 EC_KEY 数据结构 */
    auto key = EC_KEY_new();
    if (key == NULL) {
        printf("EC_KEY_new err!\n");
        return -1;
    }

    /* 设置密钥参数 */
    auto ret = EC_KEY_set_group(key, group);
    if (ret != 1) {
        printf("EC_KEY_set_group err.\n");
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

    /* 将这个点设置为密钥的公钥 */
    EC_KEY_set_public_key(key, pub_key);

    EVP_PKEY *pkey = EVP_PKEY_new();

    /* 将EC_KEY转存入EVP_PKEY中 */
    EVP_PKEY_set1_EC_KEY(pkey, key);
    /* 设置EVP_PKEY的SM2属性 */
    if ((EVP_PKEY_set_alias_type(pkey, EVP_PKEY_SM2)) != 1) {
        printf("EVP_PKEY_set_alias_type() Error.\n");
        return -1;
    }

        /* 生成EVP_PKEY_CTX上下文 */
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (!ctx)
    {
        printf("EVP_PKEY_CTX_new err\n");
        return 0;
    }
    /* 加密初始化 */
    ret = EVP_PKEY_encrypt_init(ctx);
    if (ret != 1)
    {
        printf("EVP_PKEY_encrypt_init err %d\n", ret);
        ERR_print_errors_fp(stderr);
        return 0;
    }
    /* 加密 */
    std::string data = "123456";
    unsigned char out[1024] = {0};
    unsigned char* out_ptr = out;

    size_t outlen = sizeof(out);
    EVP_PKEY_encrypt(ctx, out, &outlen, (const unsigned char *)data.c_str(), data.size());
    std::string bytes((char*)out, outlen);
    std::cout << "Encrypted data: " << byte_to_hex(bytes) << std::endl;

        /* ========= SM2密文解析处理部分 ========= */
    /* openssl定义的sm2密文结构体,我直接拿来用 */
    struct SM2_Ciphertext_st *sm2_ctext = NULL;

    /* 反序列化,把原本的密文字符串解析到SM2密文结构体中 */
    /* 序列化的话用函数int ASN1_item_i2d(const ASN1_VALUE *val, unsigned char **out, const ASN1_ITEM *it) */
    /* 这俩函数分别在tasn_dec.c和tasn_enc.c中 */
    sm2_ctext = (SM2_Ciphertext *)ASN1_item_d2i(NULL, (const unsigned char **)&out_ptr, outlen, ASN1_ITEM_rptr(SM2_Ciphertext));
    if (sm2_ctext == NULL)
    {
        printf("Resolve SM2 EncData error.\n");
        return -1;
    }
    std::string c1c2c3_hex = BN_bn2hex(sm2_ctext->C1x);
    c1c2c3_hex += BN_bn2hex(sm2_ctext->C1y);
    c1c2c3_hex += byte_to_hex(
        std::string((char*)sm2_ctext->C2->data, sm2_ctext->C2->length));
    c1c2c3_hex += byte_to_hex(
        std::string((char*)sm2_ctext->C3->data, sm2_ctext->C3->length));
    std::cout << "Parsed SM2 ciphertext: " << c1c2c3_hex << std::endl;

    SM2_Ciphertext_free(sm2_ctext);
    return 0;
}

int main()
{
    func1();
    return 0;
}
