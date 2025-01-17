#include "openssl/ec.h"
#include "openssl/evp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************************************
* Main function return value:
    0:   main function executes successfully
    -1:  an error occurs
**************************************************/
int main(void)
{
    int ret = -1, i;
    EVP_PKEY_CTX *pctx = NULL, *ectx = NULL;
    EVP_PKEY* pkey = NULL;
    unsigned char message[16] = {0x0,
                                 0x1,
                                 0x2,
                                 0x3,
                                 0x4,
                                 0x5,
                                 0x6,
                                 0x7,
                                 0x8,
                                 0x9,
                                 0xA,
                                 0xB,
                                 0xC,
                                 0xD,
                                 0xE,
                                 0xF};
    size_t message_len = sizeof(message);
    unsigned char *ciphertext = NULL, *plaintext = NULL;
    size_t ciphertext_len, plaintext_len;
    EC_KEY* key_pair = NULL;
    const BIGNUM* priv_key = NULL;
    char* priv_key_str = NULL;

    const EC_GROUP* group = NULL;
    const EC_POINT* pub_key = NULL;
    BN_CTX* ctx = NULL;
    BIGNUM *x_coordinate = NULL, *y_coordinate = NULL;
    char *x_coordinate_str = NULL, *y_coordinate_str = NULL;

    /* create SM2 Ellipse Curve parameters and key pair */
    if (!(pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL))) {
        goto clean_up;
    }

    if ((EVP_PKEY_paramgen_init(pctx)) != 1) {
        goto clean_up;
    }

    if ((EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, NID_sm2)) <= 0) {
        goto clean_up;
    }

    if ((EVP_PKEY_keygen_init(pctx)) != 1) {
        goto clean_up;
    }

    if ((EVP_PKEY_keygen(pctx, &pkey)) != 1) {
        goto clean_up;
    }

    /* print SM2 key pair */
    if (!(key_pair = EVP_PKEY_get0_EC_KEY(pkey))) {
        goto clean_up;
    }

    if (!(priv_key = EC_KEY_get0_private_key(key_pair))) {
        goto clean_up;
    }

    if (!(priv_key_str = BN_bn2hex(priv_key))) {
        goto clean_up;
    }
    printf("SM2 private key (in hex form):\n");
    printf("%s\n\n", priv_key_str);

    if (!(pub_key = EC_KEY_get0_public_key(key_pair))) {
        goto clean_up;
    }

    if (!(group = EC_KEY_get0_group(key_pair))) {
        goto clean_up;
    }

    if (!(ctx = BN_CTX_new())) {
        goto clean_up;
    }
    BN_CTX_start(ctx);
    x_coordinate = BN_CTX_get(ctx);
    y_coordinate = BN_CTX_get(ctx);
    if (!(y_coordinate)) {
        goto clean_up;
    }
    if (!(EC_POINT_get_affine_coordinates(group,
                                          pub_key,
                                          x_coordinate,
                                          y_coordinate,
                                          ctx))) {
        goto clean_up;
    }
    if (!(x_coordinate_str = BN_bn2hex(x_coordinate))) {
        goto clean_up;
    }
    printf("x coordinate in SM2 public key (in hex form):\n");
    printf("%s\n\n", x_coordinate_str);

    if (!(y_coordinate_str = BN_bn2hex(y_coordinate))) {
        goto clean_up;
    }
    printf("y coordinate in SM2 public key (in hex form):\n");
    printf("%s\n\n", y_coordinate_str);

    /* compute SM2 encryption */
    if ((EVP_PKEY_set_alias_type(pkey, EVP_PKEY_SM2)) != 1) {
        goto clean_up;
    }

    if (!(ectx = EVP_PKEY_CTX_new(pkey, NULL))) {
        goto clean_up;
    }

    if ((EVP_PKEY_encrypt_init(ectx)) != 1) {
        goto clean_up;
    }

    if ((EVP_PKEY_encrypt(ectx, NULL, &ciphertext_len, message, message_len)) !=
        1) {
        goto clean_up;
    }

    if (!(ciphertext = (unsigned char*)malloc(ciphertext_len))) {
        goto clean_up;
    }

    if ((EVP_PKEY_encrypt(ectx,
                          ciphertext,
                          &ciphertext_len,
                          message,
                          message_len)) != 1) {
        goto clean_up;
    }

    printf("Message length: %ld bytes.\n", message_len);
    printf("Message:\n");
    for (i = 0; i < (int)message_len; i++) {
        printf("0x%x  ", message[i]);
    }
    printf("\n\n");

    printf("Ciphertext length: %ld bytes.\n", ciphertext_len);
    printf("Ciphertext (ASN.1 encode):\n");
    for (i = 0; i < (int)ciphertext_len; i++) {
        printf("0x%x  ", ciphertext[i]);
    }
    printf("\n\n");

    /* compute SM2 decryption */
    if ((EVP_PKEY_decrypt_init(ectx)) != 1) {
        goto clean_up;
    }

    if ((EVP_PKEY_decrypt(ectx,
                          NULL,
                          &plaintext_len,
                          ciphertext,
                          ciphertext_len)) != 1) {
        goto clean_up;
    }

    if (!(plaintext = (unsigned char*)malloc(plaintext_len))) {
        goto clean_up;
    }

    if ((EVP_PKEY_decrypt(ectx,
                          plaintext,
                          &plaintext_len,
                          ciphertext,
                          ciphertext_len)) != 1) {
        goto clean_up;
    }

    printf("Decrypted plaintext length: %ld bytes.\n", plaintext_len);
    printf("Decrypted plaintext:\n");
    for (i = 0; i < (int)plaintext_len; i++) {
        printf("0x%x  ", plaintext[i]);
    }
    printf("\n\n");

    if (plaintext_len != message_len) {
        printf("Decrypted data length error!\n");
        goto clean_up;
    }

    if (memcmp(plaintext, message, message_len)) {
        printf("Decrypt data failed!\n");
        goto clean_up;
    } else {
        printf("Encrypt and decrypt data succeeded!\n");
    }

    ret = 0;
clean_up:
    if (pctx) {
        EVP_PKEY_CTX_free(pctx);
    }

    if (pkey) {
        EVP_PKEY_free(pkey);
    }

    if (priv_key_str) {
        OPENSSL_free(priv_key_str);
    }

    if (ctx) {
        BN_CTX_end(ctx);
        BN_CTX_free(ctx);
    }

    if (x_coordinate_str) {
        OPENSSL_free(x_coordinate_str);
    }

    if (y_coordinate_str) {
        OPENSSL_free(y_coordinate_str);
    }

    if (ectx) {
        EVP_PKEY_CTX_free(ectx);
    }

    if (ciphertext) {
        free(ciphertext);
    }

    if (plaintext) {
        free(plaintext);
    }

#if defined(_WIN32) || defined(_WIN64)
    system("pause");
#endif
    return ret;
}
