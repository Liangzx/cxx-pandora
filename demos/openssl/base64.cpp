#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/evp.h>


//int string2base64(char *input, int input_len, char **base64) {
//    EVP_ENCODE_CTX ectx;
//    EVP_EncodeInit(&ectx);
//    int out_len = 0;
//    if (input_len % 3 == 0) {
//        out_len = input_len / 3 * 4;
//    } else {
//        out_len = (input_len / 3 + 1) * 4;
//    }
//    // 数据够长的话会有多个换行
//    char *out = (char *)malloc(out_len + out_len/80 +1);
//    int total = 0;
//    int encode_len = 0;
//    EVP_EncodeUpdate(&ectx,out,&encode_len,input,input_len);
//    total += encode_len;
//    EVP_EncodeFinal(&ectx,out+total,&encode_len);
//    total += encode_len;
//
//    printf("%s",out);
//    *base64 = out;
//    return total;
//}

void base64encode_stream() {
    const char *input = "1234567890abcdefg";
    int inl = strlen(input);

    auto *out = (unsigned char *) malloc(inl * 2);
    memset(out, 0, inl * 2);

    EVP_ENCODE_CTX *evp_ctx = EVP_ENCODE_CTX_new();
    EVP_EncodeInit(evp_ctx);

    int total = 0;
    int outl = 0;

    int ret = EVP_EncodeUpdate(evp_ctx, out, &outl, (const unsigned char *) input, inl);
    total += outl;
    EVP_EncodeFinal(evp_ctx,out+total,&outl);
    total += outl;
    EVP_ENCODE_CTX_free(evp_ctx);

    printf("outl=%d out=%s\n",total, out);
    free(out);
}

void base64decode_stream() {
    const char *input = "MTIzNDU2Nzg5MGFiY2RlZmc=";
    int inl = strlen(input);
    auto *out = (unsigned char *) malloc(inl);

    EVP_ENCODE_CTX *evp_ctx = EVP_ENCODE_CTX_new();
    EVP_DecodeInit(evp_ctx);
    int total = 0;
    int outl = 0;

    EVP_DecodeUpdate(evp_ctx, out, &outl, (const unsigned char *)input, inl);
    total += outl;
    EVP_DecodeFinal(evp_ctx, out + total, &outl);
    total += outl;
    EVP_ENCODE_CTX_free(evp_ctx);

    printf("outl=%d out=%s\n",total, out);
    free(out);
}

using std::string<unsigned char> bytes;

void base64encode_block() {
    const char *input = "1234567890abcdefg";
    int inl = strlen(input);
    auto out = (unsigned char *) malloc(inl * 2);
    EVP_EncodeBlock(out, (const unsigned char*)input, inl);
    printf("out=%s\n", out);
    free(out);
}

void base64decode_block() {
    const char *input = "MTIzNDU2Nzg5MGFiY2RlZmc=";
    int inl = strlen(input);
    auto out = (unsigned char *) malloc(inl * 2);
    EVP_DecodeBlock(out, (const unsigned char*)input, inl);
    printf("out=%s\n", out);
    free(out);
}

int main(int argc, char *argv[]) {
    base64encode_stream();
    base64decode_stream();
    base64encode_block();
    base64decode_block();

    return 0;
}