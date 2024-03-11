[OpenSSL中文手册之EVP库详解]("https://blog.csdn.net/liao20081228/article/details/76285896")

```rsa
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <iostream>

int main() {
    // 加载公钥
    EVP_PKEY* publicKey = EVP_PKEY_new();
    FILE* publicKeyFile = fopen("publickey.pem", "rb");
    if (!PEM_read_PUBKEY(publicKeyFile, &publicKey, nullptr, nullptr)) {
        std::cerr << "Failed to load public key" << std::endl;
        return 1;
    }
    fclose(publicKeyFile);

    // 创建加密上下文
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    // 设置加密算法和公钥
    EVP_PKEY_CTX* pkeyCtx = EVP_PKEY_CTX_new(publicKey, nullptr);
    EVP_PKEY_encrypt_init(pkeyCtx);

    // 获取密文长度
    size_t encryptedLength;
    EVP_PKEY_encrypt(pkeyCtx, nullptr, &encryptedLength, (const unsigned char*)"Hello, World!", sizeof("Hello, World!"));

    // 加密明文
    unsigned char* encryptedText = new unsigned char[encryptedLength];
    EVP_PKEY_encrypt(pkeyCtx, encryptedText, &encryptedLength, (const unsigned char*)"Hello, World!", sizeof("Hello, World!"));

    // 输出加密后的密文
    std::cout << "Encrypted Text: ";
    for (size_t i = 0; i < encryptedLength; i++) {
        std::cout << std::hex << (int)encryptedText[i];
    }
    std::cout << std::endl;

    // 释放资源
    EVP_PKEY_CTX_free(pkeyCtx);
    EVP_PKEY_free(publicKey);
    EVP_CIPHER_CTX_free(ctx);
    delete[] encryptedText;

    return 0;
}
```


```
bool SangforSCP_Host_Handler::encrypt_string_with_public_key(const string & uuid, string & encrypted_str) {
    Agent agent;
    Agent_Manager::instance()->get_host(conn, uuid, agent);
    agent.public_key_
}
```
