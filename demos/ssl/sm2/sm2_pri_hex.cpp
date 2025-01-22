#include <cstring>
#include <iostream>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/sm2.h>
#include <vector>

// 将十六进制字符串转换为二进制数据
std::vector<unsigned char> hex_to_bytes(const std::string& hex)
{
  std::vector<unsigned char> bytes;
  for (size_t i = 0; i < hex.length(); i += 2) {
    std::string byteString = hex.substr(i, 2);
    unsigned char byte =
      static_cast<unsigned char>(strtol(byteString.c_str(), nullptr, 16));
    bytes.push_back(byte);
  }
  return bytes;
}

// 从十六进制私钥重建 EVP_PKEY
EVP_PKEY* rebuild_sm2_private_key(const std::string& private_key_hex)
{
  // 将十六进制私钥转换为二进制数据
  std::vector<unsigned char> private_key_bytes = hex_to_bytes(private_key_hex);
  const unsigned char* priv_key_data = private_key_bytes.data();
  size_t priv_key_len = private_key_bytes.size();

  // 创建 BIGNUM 对象
  BIGNUM* priv_key_bn = BN_bin2bn(priv_key_data, priv_key_len, nullptr);
  if (!priv_key_bn) {
    std::cerr << "Failed to create BIGNUM from private key" << std::endl;
    return nullptr;
  }

  // 创建 EC_KEY 对象
  EC_KEY* ec_key = EC_KEY_new_by_curve_name(NID_sm2);
  if (!ec_key) {
    std::cerr << "Failed to create EC_KEY for SM2" << std::endl;
    BN_free(priv_key_bn);
    return nullptr;
  }

  // 设置私钥
  if (EC_KEY_set_private_key(ec_key, priv_key_bn) != 1) {
    std::cerr << "Failed to set private key" << std::endl;
    EC_KEY_free(ec_key);
    BN_free(priv_key_bn);
    return nullptr;
  }

  // 生成公钥
  if (EC_KEY_generate_key(ec_key) != 1) {
    std::cerr << "Failed to generate public key" << std::endl;
    EC_KEY_free(ec_key);
    BN_free(priv_key_bn);
    return nullptr;
  }

  // 创建 EVP_PKEY 对象
  EVP_PKEY* pkey = EVP_PKEY_new();
  if (!pkey) {
    std::cerr << "Failed to create EVP_PKEY" << std::endl;
    EC_KEY_free(ec_key);
    BN_free(priv_key_bn);
    return nullptr;
  }

  // 将 EC_KEY 赋值给 EVP_PKEY
  if (EVP_PKEY_assign_EC_KEY(pkey, ec_key) != 1) {
    std::cerr << "Failed to assign EC_KEY to EVP_PKEY" << std::endl;
    EVP_PKEY_free(pkey);
    EC_KEY_free(ec_key);
    BN_free(priv_key_bn);
    return nullptr;
  }

  // 清理资源
  BN_free(priv_key_bn);

  return pkey;
}

int main()
{
  // 已知的 SM2 私钥（十六进制）
  std::string private_key_hex =
    "289c2d6f7d8f4a1b9e2c8d7a6b5c4d3e2f1a0b9e8d7c6b5a4f3e2d1c0b9a8";

  // 重建 EVP_PKEY
  EVP_PKEY* pkey = rebuild_sm2_private_key(private_key_hex);
  if (!pkey) {
    std::cerr << "Failed to rebuild SM2 private key" << std::endl;
    return 1;
  }

  // 打印私钥信息（PEM 格式）
  PEM_write_PrivateKey(stdout, pkey, nullptr, nullptr, 0, nullptr, nullptr);

  // 释放资源
  EVP_PKEY_free(pkey);

  return 0;
}
