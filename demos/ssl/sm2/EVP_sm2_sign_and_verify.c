/**************************************************
* File name: EVP_sm2_sign_and_verify.c
* Author: HAN Wei
* Author's blog: https://blog.csdn.net/henter/
* Date: April 27th, 2020
* Description: demonstrate how to compute SM2 signature
        and verify it by invoking EVP interface
        in OpenSSL 1.1.1
**************************************************/
// https://www.cnblogs.com/wqnmlkb/p/14831510.html
#include "openssl/ec.h"
#include "openssl/evp.h"
#include <stdio.h>
#include <stdlib.h>

/**************************************************
* Main function return value:
    0:   main function executes successfully
    -1:  an error occurs
**************************************************/
int main(void)
{
    int ret = -1, i;
    EVP_PKEY_CTX *pctx = NULL, *sctx = NULL;
    EVP_PKEY* pkey = NULL;
    EVP_MD_CTX *md_ctx = NULL, *md_ctx_verify = NULL;
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
    unsigned char* sig = NULL;
    size_t sig_len;
    unsigned char sm2_id[] = {0x31,
                              0x32,
                              0x33,
                              0x34,
                              0x35,
                              0x36,
                              0x37,
                              0x38,
                              0x31,
                              0x32,
                              0x33,
                              0x34,
                              0x35,
                              0x36,
                              0x37,
                              0x38};
    unsigned int sm2_id_len = sizeof(sm2_id);
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

    /* compute SM2 signature */
    if ((EVP_PKEY_set_alias_type(pkey, EVP_PKEY_SM2)) != 1) {
        goto clean_up;
    }

    if (!(md_ctx = EVP_MD_CTX_new())) {
        goto clean_up;
    }

    if (!(sctx = EVP_PKEY_CTX_new(pkey, NULL))) {
        goto clean_up;
    }

    if (EVP_PKEY_CTX_set1_id(sctx, sm2_id, sm2_id_len) <= 0) {
        goto clean_up;
    }

    EVP_MD_CTX_set_pkey_ctx(md_ctx, sctx);

    if ((EVP_DigestSignInit(md_ctx, NULL, EVP_sm3(), NULL, pkey)) != 1) {
        goto clean_up;
    }

    if ((EVP_DigestSignUpdate(md_ctx, message, message_len)) != 1) {
        goto clean_up;
    }

    if ((EVP_DigestSignFinal(md_ctx, NULL, &sig_len)) != 1) {
        goto clean_up;
    }

    if (!(sig = (unsigned char*)malloc(sig_len))) {
        goto clean_up;
    }

    if ((EVP_DigestSignFinal(md_ctx, sig, &sig_len)) != 1) {
        goto clean_up;
    }

    printf("Message length: %ld bytes.\n", message_len);
    printf("Message:\n");
    for (i = 0; i < (int)message_len; i++) {
        printf("0x%x  ", message[i]);
    }
    printf("\n\n");

    printf("SM2 signature length: %ld bytes.\n", sig_len);
    printf("SM2 signature (ASN.1 encode):\n");
    for (i = 0; i < (int)sig_len; i++) {
        printf("0x%x  ", sig[i]);
    }
    printf("\n\n");

    /* verify SM2 signature */
    if (!(md_ctx_verify = EVP_MD_CTX_new())) {
        goto clean_up;
    }

    EVP_MD_CTX_set_pkey_ctx(md_ctx_verify, sctx);

    if ((EVP_DigestVerifyInit(md_ctx_verify, NULL, EVP_sm3(), NULL, pkey)) !=
        1) {
        goto clean_up;
    }

    if ((EVP_DigestVerifyUpdate(md_ctx_verify, message, sizeof(message))) !=
        1) {
        goto clean_up;
    }

    if ((EVP_DigestVerifyFinal(md_ctx_verify, sig, sig_len)) != 1) {
        printf("Verify SM2 signature failed!\n");
        goto clean_up;
    } else {
        printf("Verify SM2 signature succeeded!\n");
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

    if (md_ctx) {
        EVP_MD_CTX_free(md_ctx);
    }

    if (sctx) {
        EVP_PKEY_CTX_free(sctx);
    }

    if (sig) {
        free(sig);
    }

    if (md_ctx_verify) {
        EVP_MD_CTX_free(md_ctx_verify);
    }

#if defined(_WIN32) || defined(_WIN64)
    system("pause");
#endif
    return ret;
}
