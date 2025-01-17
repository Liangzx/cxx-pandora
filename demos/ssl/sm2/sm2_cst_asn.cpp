#include <string.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/ecdsa.h>
#include <openssl/objects.h>
#include <openssl/err.h>
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

int main()
{
    EC_KEY *key1, *key2;
    const EC_POINT *pubkey1;
    EC_POINT *pubkey2;
    EC_GROUP *group1, *group2;
    unsigned int ret, nid, size, i, sig_len;
    unsigned char *signature, digest[20];
    BIO *berr;
    EC_builtin_curve *curves;
    int crv_len;
    char shareKey1[128], shareKey2[128];
    int len1, len2;
    /* 构造 EC_KEY 数据结构 */
    key1 = EC_KEY_new();
    if (key1 == NULL)
    {
        printf("EC_KEY_new err!\n");
        return -1;
    }
    key2 = EC_KEY_new();
    if (key2 == NULL)
    {
        printf("EC_KEY_new err!\n");
        return -1;
    }
    /* 获取实现的椭圆曲线个数 */
    crv_len = EC_get_builtin_curves(NULL, 0);
    curves = (EC_builtin_curve *)malloc(sizeof(EC_builtin_curve) * crv_len);
    /* 获取椭圆曲线列表 */
    EC_get_builtin_curves(curves, crv_len);
    /*
    nid=curves[0].nid;会有错误，原因是密钥太短
    */
    /* 选取一种椭圆曲线 */
    /* SM2貌似是1172,需要验证. 要么就得在comments中判断 */
    nid = 1172; // curves[25].nid;

    /* 根据选择的椭圆曲线生成密钥参数 group */
    group1 = EC_GROUP_new_by_curve_name(nid);
    if (group1 == NULL)
    {
        printf("EC_GROUP_new_by_curve_name err!\n");
        return -1;
    }
    group2 = EC_GROUP_new_by_curve_name(nid);
    if (group1 == NULL)
    {
        printf("EC_GROUP_new_by_curve_name err!\n");
        return -1;
    }
    /* 设置密钥参数 */
    ret = EC_KEY_set_group(key1, group1);
    if (ret != 1)
    {
        printf("EC_KEY_set_group err.\n");
        return -1;
    }
    ret = EC_KEY_set_group(key2, group2);
    if (ret != 1)
    {
        printf("EC_KEY_set_group err.\n");
        return -1;
    }
    /* 生成密钥 */
    ret = EC_KEY_generate_key(key1);
    if (ret != 1)
    {
        printf("EC_KEY_generate_key err.\n");
        return -1;
    }
    /* 这里为什么注释?因为在下面获取key2的公钥指针时候,
     * 愕然发现这个函数不需要key2初始化,但是有的函数就需要否则会coredump,很离谱
     * 见下面 注释的EC_KEY_get0_public_key函数 和 EC_KEY_set_public_key函数
     */
    // ret = EC_KEY_generate_key(key2);
    // if (ret != 1)
    // {
    //     printf("EC_KEY_generate_key err.\n");
    //     return -1;
    // }
    /* 检查密钥 */
    ret = EC_KEY_check_key(key1);
    if (ret != 1)
    {
        printf("check key err.\n");
        return -1;
    }
    /* 获取密钥大小 */
    size = ECDSA_size(key1);
    printf("size %d \n", size);
    for (i = 0; i < 20; i++)
        memset(&digest[i], i + 1, 1);
    signature = (unsigned char *)malloc(size);
    ERR_load_crypto_strings();
    berr = BIO_new(BIO_s_file());
    // BIO_set_fp(berr,stdout,BIO_NOCLOSE);

    /* 签名数据，本例未做摘要，可将 digest 中的数据看作是 sha1 摘要结果 */
    /*************** 务必注意，这个函数在openssl3.0下可以正常使用，但在1.1.1下这个函数就只是ECDSA算法的签名而已，不是SM2，1.1.1下的SM2算法需要使用EVP_PKEY_**系列函数才能调用到。具体调用方式参见下面的验签部分。这里我就保留做样例了，需要1.1.1代码的参考下面代码挪动到这里修改下就可以了 ***************/
    /* 另外需要注意的一点是，openssl使用的数据，就是digest的部分，是bin格式的，不是hex格式的，即char digest[]={0x01,0x23,...}而不是char digest[]=“0123...” */
    ret = ECDSA_sign(0, digest, 20, signature, &sig_len, key1);
    if (ret != 1)
    {
        ERR_print_errors(berr);
        printf("sign err!\n");
        return -1;
    }

    /* 从key1中提取公钥,注意两个变量的类型定义 */
    pubkey1 = EC_KEY_get0_public_key(key1);
    /* 这个函数定义在ec_key.c中,这个函数上面一个就是get_private_key,如有需要自己研究怎么用吧 */

    /* 公钥结构定义中包含x/y的BIGNUM变量,这里创建两个变量进行承接 */
    BIGNUM *x = BN_new();
    BIGNUM *y = BN_new();
    /* 承接转字符串后的x和y,名字我瞎JB起的,别在意,反正是demo */
    char *xx;
    char *yy;

    /* 核心的提取函数,将公钥结构中的x和y提取到后面的两个参数中,类型为BIGNUM,后面注释的函数功能是逆向将XY赋值到公钥中 */
    if (EC_POINT_get_affine_coordinates(group1, pubkey1, x, y, NULL)) // EC_POINT_set_affine_coordinates
    {
        /* 这里输出了BIGNUM形式的X和Y,并一起以16进制字符串形式赋值给字符指针xx,yy. */
        BN_print_fp(stdout, x);
        xx = BN_bn2hex(x);
        putc('\n', stdout);
        printf("%s\n", xx);
        // 1392EBCB4CCC845F7DEB74DA0336531D47F75711244ED96100B0D632D61E7780
        BN_print_fp(stdout, y);
        yy = BN_bn2hex(y);
        putc('\n', stdout);
        printf("%s\n", yy);
    }
    return 0;

    /* 进行密钥的传递和转换,这里是承接XY16进制字符串的字符串空间 */
    char x2[128] = {0};
    char y2[128] = {0};

    /* 密钥传递,我前面使用的nid为1172,即SM2,相应的XY为64字节 */
    snprintf(x2, 128, "%s", xx);
    snprintf(y2, 128, "%s", yy);
    /* 看看对不对 */
    printf("x2:%s.\ny2:%s.\n\n", x2, y2);

    /* 字符串转BIGNUM,就是前面公钥提取的逆过程,这里是初始的xy定义,2代表这是承接者 */
    BIGNUM *BNx2 = NULL;
    BIGNUM *BNy2 = NULL;
    /* 字符串转BIGNUM核心函数 */
    BN_hex2bn(&BNx2, x2);
    BN_hex2bn(&BNy2, y2);
    /* 看看对不对 */
    BN_print_fp(stdout, BNx2);
    putc('\n', stdout);
    BN_print_fp(stdout, BNy2);
    putc('\n', stdout);

    /* 注意这里是要将BIGNUM转换为公钥,
     * 公钥本质上就是椭圆曲线上的一个点,
     * 所以这里就是在曲线上取一个点,当然这个点目前是没有确定的,空点
     * 在曲线2上取,因为这个代码是公钥序列化导出和导入,所以不会直接用原来的1曲线
     * 当然了,因为是同一个算法,所以即使是两条曲线,也是完全相同的
     * 后面注释的内容是另一个方法,从曲线上导出一个密钥,再从密钥中拿到公钥的指针,再用咱的公钥覆盖
     * 这里就不用了,用更加官方的导入方式
     */
    pubkey2 = EC_POINT_new(group2); // EC_KEY_get0_public_key(key2);

    /* 将BIGNUM类型的X和Y赋值到获取的点pubkey2中 */
    EC_POINT_set_affine_coordinates(group2, pubkey2, BNx2, BNy2, NULL);
    /* 将这个点设置为密钥的公钥 */
    EC_KEY_set_public_key(key2, pubkey2);

    /* 至此将Key1中的公钥序列化导出,再重新反序列化导入的过程就结束了 */

    /* 验证签名 */
    /* 注意这个验签是openssl3.0的，所以我注释了 */
    //ret = ECDSA_verify(0, digest, 20, signature, sig_len, key2);

    /********* openssl 1.1.1可用的验签代码 *********/
    /* 创建EVP_PKEY */
    EVP_PKEY *pkeySM2 = EVP_PKEY_new();
    if (!pkeySM2)
    {
        printf("EVP_PKEY_new() Error.\n");
        return -1;
    }
    /* 将EC_KEY转存入EVP_PKEY中 */
    EVP_PKEY_set1_EC_KEY(pkeySM2, key2);
    /* 设置EVP_PKEY的SM2属性 */
    if ((EVP_PKEY_set_alias_type(pkeySM2, EVP_PKEY_SM2)) != 1)
    {
        printf("EVP_PKEY_set_alias_type() Error.\n");
        return -1;
    }
    /* 创建EVP_PKEY_CTX变量 */
    EVP_PKEY_CTX *ctxSM2 = EVP_PKEY_CTX_new(pkeySM2, NULL);
    if (!ctxSM2)
    {
        printf("EVP_PKEY_CTX_new() Error.\n");
        return -1;
    }
    /* 初始化EVP验签环境 */
    ret = EVP_PKEY_verify_init(ctxSM2);
    if (ret != 1)
    {
        printf("EVP_PKEY_verify_init() Error %d.\n", ret);
        return -1;
    }
    /* 验证签名 */
    ret = EVP_PKEY_verify(ctxSM2, signature, sig_len, digest, 20);
    /************ openssl 1.1.1版本的验签代码完毕 ************/
    if (ret != 1)
    {
        ERR_print_errors(berr);
        printf("ECDSA_verify err!\n");
        return -1;
    }
    printf("ECDSA_verify OK!\n");

    /* 签名的序列化导出与导入,值得注意的是,签名 signature 是个纯字符串*/
    /* 这里先检查该签名的长度,sm2的在69到72之间,其中包含r+s 64的签名和其他标识符之类的内容 */
    printf("Sig %d \n\n", sig_len);

    /* 这是我自己写的,识别标识符(标识长度的部分)并切分显示r和s的代码,当然了,没做其他处理,因为好几个字节我不知道什么意思不敢删 */
    int cou = 0;
    for (cou = 0; cou < signature[3] + 4; cou++)
        printf("%02x ", signature[cou]);
    printf("\n");
    for (cou = 0; cou < signature[1] + 2 - signature[3] - 4; cou++)
        printf("%02x ", signature[signature[3] + 4 + cou]);

    printf("\n");

    /* 创建空的签名类型变量,用以承载解码后的r和s */
    ECDSA_SIG *s;

    s = ECDSA_SIG_new();
    if (s == NULL)
        return ret;

    /* 将signature解码到s中,需要注意的是,这个解码函数会一并将signature释放,因此下面的free就不需要了,会报错 */
    if (d2i_ECDSA_SIG(&s, &signature, sig_len) == NULL) // i2d_ECDSA_SIG
        return ret;

    /* 老黄历,创建两个BIGNUM用以分别承载r和s */
    BIGNUM *BNr = NULL;
    BIGNUM *BNs = NULL;

    /* 从签名类型变量s中解码出r和s到BIGNUM中,后面注释的函数是逆向流程 */
    ECDSA_SIG_get0(s, &BNr, &BNs); // int ECDSA_SIG_set0(ECDSA_SIG *sig, BIGNUM *r, BIGNUM *s);
    /* 检查看看对不对,这里是没有标识符字节的版本 */
    BN_print_fp(stdout, BNr);
    putc('\n', stdout);
    BN_print_fp(stdout, BNs);
    putc('\n', stdout);
    /* 这里按照前面调用bn2hex就可以弄成字符串了 */

    /* 将字符串/BIGNUM重新反序列化回签名类型变量就不赘述了,建好用到的变量用前面注释的函数逆向一遍就行 */
    /* 首先创建字符串空间
     * 为什么建立两个指针?因为下面那个二逼反序列化函数会把指针挪到字符串结尾,
     * 要么像我一样多建一个保存头,要么就得自己再-length挪动回来
     */
    unsigned char *ssssig = calloc(1, 72);
    unsigned char *zzzzzzz = ssssig;
    int ssslen = 0;
    /* 将签名类型变量s反序列化成字符串 */
    ssslen = i2d_ECDSA_SIG(s, &ssssig);

    /* 检查下长度对不对 */
    printf("Sig %d \n\n", ssslen);

    /* 再输出一遍,看看和前面那个输出是否一致,尤其是标识符和总长度 */
    for (cou = 0; cou < 72; cou++)
        printf("%02x ", zzzzzzz[cou]);
    printf("\n");
    for (cou = 0; cou < zzzzzzz[1] + 2 - zzzzzzz[3] - 4; cou++)
        printf("%02x ", zzzzzzz[zzzzzzz[3] + 4 + cou]);

    /* 弄回字符串了就可以继续验签之类的了 */

    /* 序列化反序列化后的密钥加解密效果测试,加密使用了承载序列化数据的自定义密钥key2,解密使用原始密钥,编译后执行的效果可以看出是正常的 */
    //===============================================加解密部分========================================================
    /* 将密钥存在EVP_PKEY中 */
    /* 建立新的密钥变量 */
    EVP_PKEY *pkey = EVP_PKEY_new();
    /* ECkey转存为密钥 */
    EVP_PKEY_set1_EC_KEY(pkey, key2);
    /* 设置密钥属性,注意这一步是必须的,不设置属性的话下面调用加解密函数会提示不支持该操作.猜测这里面根据指定的属性初始化一系列相关属性/操作函数的指针之类的内容 */
    if ((EVP_PKEY_set_alias_type(pkey, EVP_PKEY_SM2)) != 1)
    {
        printf("EVP_PKEY_set_alias_type err\n");
        ERR_print_errors_fp(stderr);
        return 0;
    }
    /* 数据部分,测试用 */
    unsigned char data[] = "1234567890TESTDATA!@#$%^&*()ASDFGHJKL";
    unsigned char out[1024] = {0};
    unsigned char out2[1024] = {0};
    int datasize = sizeof(data);
    printf("EVP_DATA len:%d  Out:%s.\n", datasize, data);
    printf("=================加密部分===============\n");

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
    size_t outlen = sizeof(out);
    EVP_PKEY_encrypt(ctx, out, &outlen, data, datasize);
    printf("EVP_ENC  len:%d  Out:%s.\n\n\n\n\n\n", outlen, out);

    //===================================
    /* 将密钥存在EVP_PKEY中 */
    /* 这里是初始化解密的环境,注意使用了原始EC密钥key1,和上面的key2不同.这是为了验证EC密钥序列化反序列化操作的正确性,同时证明加解密操作只会使用对应的公私钥,另一个(私/公)密钥为空不影响操作(key2的私钥是空的,而key1的公私钥是完整的.但结果是正确的,说明不强制要求密钥的完整性) */
    printf("=================解密部分===============\n");
    EVP_PKEY *pkey1 = EVP_PKEY_new();
    EVP_PKEY_set1_EC_KEY(pkey1, key1);
    if ((EVP_PKEY_set_alias_type(pkey1, EVP_PKEY_SM2)) != 1)
    {
        printf("EVP_PKEY_set_alias_type err\n");
        ERR_print_errors_fp(stderr);
        return 0;
    }
    EVP_PKEY_CTX *ctx1 = EVP_PKEY_CTX_new(pkey1, NULL);
    if (!ctx1)
    {
        printf("EVP_PKEY_CTX_new err\n");
        return 0;
    }

    ret = EVP_PKEY_decrypt_init(ctx1);
    if (ret != 1)
    {
        printf("EVP_PKEY_encrypt_init err %d\n", ret);
        return 0;
    }
    int insize = outlen;
    outlen = sizeof(out2);
    EVP_PKEY_decrypt(ctx1, out2, &outlen, out, insize);
    printf("EVP_DEC  len:%d  Out:%s.\n\n", outlen, out2);
    printf("EVP_ENC  len:%d  Out:%s.\n\n\n\n\n\n", insize, out);

    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);
    // return 0;

    /* ========= SM2密文解析处理部分 ========= */
    /* openssl定义的sm2密文结构体,我直接拿来用 */
    struct SM2_Ciphertext_st *sm2_ctext = NULL;

    /* 反序列化,把原本的密文字符串解析到SM2密文结构体中 */
    /* 序列化的话用函数int ASN1_item_i2d(const ASN1_VALUE *val, unsigned char **out, const ASN1_ITEM *it) */
    /* 这俩函数分别在tasn_dec.c和tasn_enc.c中 */
    sm2_ctext = (SM2_Ciphertext *)ASN1_item_d2i(nullptr, (const unsigned char **)&out, insize, ASN1_ITEM_rptr(SM2_Ciphertext));
    if (sm2_ctext == NULL)
    {
        printf("Resolve SM2 EncData error.\n");
        return -1;
    }

    /* 输出内容检查一下 */
    printf("=================密文解析部分===============\n");
    /* 密文中的xy,类型为BIGNUM,相关处理参考前面的处理即可 */
    /* 输出为64字节,因为是16进制输出,实际正常应该是32字节长度 */
    printf("X:");
    BN_print_fp(stdout, sm2_ctext->C1x);
    putc('\n', stdout);
    printf("Y:");
    BN_print_fp(stdout, sm2_ctext->C1y);
    putc('\n\n\n\n', stdout);
    /* 杂凑值和密文本体的输出,这俩是openssl定义的类型,里面有四个变量,主要用到的是长度和数据本体指针 */
    /* 杂凑值,32字节 */
    printf("Hash  len %d Text:\n%s\n\n\n\n\n", sm2_ctext->C3->length, sm2_ctext->C3->data);
    /* 密文本体,长度和明文一致 */
    printf("Enced len %d Text:\n%s\n\n\n\n\n", sm2_ctext->C2->length, sm2_ctext->C2->data);
    /* SM2的密文应该是32+32+32+LenOfText才对,但是openssl的asn1序列化会加校验位和长度位(大概),所以实际长度会比理论长度长一些 */


    //================================================================================================================

    // /* 获取对方公钥，不能直接引用 */
    // pubkey2 = EC_KEY_get0_public_key(key2);
    // // EC_KEY_set_public_key

    // BIGNUM *x = BN_new();
    // BIGNUM *y = BN_new();
    // char *xx; //[1024] = {0};
    // char *yy; //[1024] = {0};

    // if (EC_POINT_get_affine_coordinates(group2, pubkey2, x, y, NULL)) // EC_POINT_set_affine_coordinates
    // {
    //     BN_print_fp(stdout, x);
    //     xx = BN_bn2hex(x);
    //     putc('\n', stdout);
    //     printf("%s\n", xx);

    //     BN_print_fp(stdout, y);
    //     yy = BN_bn2hex(y);
    //     putc('\n', stdout);
    //     printf("%s\n", yy);
    // }

    // /* 生成一方的共享密钥 */
    // len1 = ECDH_compute_key(shareKey1, 128, pubkey2, key1, NULL);
    // pubkey1 = EC_KEY_get0_public_key(key1);
    // /* 生成另一方共享密钥 */
    // len2 = ECDH_compute_key(shareKey2, 128, pubkey1, key2, NULL);
    // if (len1 != len2)
    // {
    //     printf("err\n");
    // }
    // else
    // {
    //     ret = memcmp(shareKey1, shareKey2, len1);
    //     if (ret == 0)
    //         printf("生成共享密钥成功\n");
    //     else
    //         printf("生成共享密钥失败\n");
    // }
    // printf("test ok!\n");
    BIO_free(berr);
    EC_KEY_free(key1);
    EC_KEY_free(key2);
    // free(signature);
    free(curves);
    return 0;
}
