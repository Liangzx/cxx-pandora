/*
 * 这个文件主要展示怎样通过yajl生成json语句。
 * 这里不包括解析的部分，仅仅包括了最简单的构造部分。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yajl/yajl_gen.h>

// #define USE_MAP 1
#define BEAUTIFY_OUTPUT 1



typedef struct yajl_info_t {
  int depth;
  yajl_gen gen;  // 保存一个generator
} yajl_info;

/*
 * begin_map和end_map会生成一个对象
 * 在它们直接插入数据的话，会变成对象中的键/值对
 */
yajl_gen_status begin_map(void *ctx) {
  yajl_info *info = (yajl_info *)ctx;
  yajl_gen_status __stat = yajl_gen_map_open(info->gen);
  if (__stat == yajl_gen_status_ok) {
    info->depth++;
  } else {
    printf("yajl_gen_map_open failed\n");
  }
  return __stat;
}

yajl_gen_status end_map(void *ctx) {
  yajl_info *info = (yajl_info *)ctx;
  yajl_gen_status __stat = yajl_gen_map_close(info->gen);
  if (__stat == yajl_gen_status_ok) {
    info->depth--;
  }

  return __stat;
}

/*
 * begin_map和end_map会生成一个数组
 * 在它们直接插入数据的话，会变成数组中的内存
 */
yajl_gen_status begin_array(void *ctx) {
  yajl_info *info = (yajl_info *)ctx;
  yajl_gen_status __stat = yajl_gen_array_open(info->gen);
  if (__stat == yajl_gen_status_ok) {
    info->depth++;
  } else {
    printf("yajl_gen_array_open failed\n");
  }
  return __stat;
}

yajl_gen_status end_array(void *ctx) {
  yajl_info *info = (yajl_info *)ctx;
  yajl_gen_status __stat = yajl_gen_array_close(info->gen);
  if (__stat == yajl_gen_status_ok) {
    info->depth--;
  }
  return __stat;
}

void gen_print(void *ctx, const char *str, size_t len) {
  printf("%s", str);
}


int yajl_init(yajl_info *info) {
  info->depth = 0;
  info->gen = yajl_gen_alloc(NULL);
#ifdef BEAUTIFY_OUTPUT
  // 设置输出格式
  yajl_gen_config(info->gen, yajl_gen_beautify, 1);
#endif
  yajl_gen_config(info->gen, yajl_gen_print_callback, gen_print, NULL);
}

int yajl_free(yajl_info *info) {
  info->depth = 0;
  yajl_gen_free(info->gen);
}

int main(int argc, char **argv) {
  yajl_info yajlinfo;
  yajl_init(&yajlinfo);
  const unsigned char *buf;
  size_t buflen;
  int i;

#ifdef USE_MAP
  // 创建一个对象
  begin_map(&yajlinfo);
#else
  // 创建一个数组
  begin_array(&yajlinfo);
#endif

  // 往对象/数组中插入数据
  unsigned char *key = "scutech";
  unsigned char *value = "dingjia";
  int keylen = strlen(key);
  int valuelen = strlen(value);
  yajl_gen_string(yajlinfo.gen, (const unsigned char *)key, keylen);
  yajl_gen_bool(yajlinfo.gen, 1);
  //   yajl_gen_string(yajlinfo.gen, (const unsigned char *)value, valuelen);

  for (i = 1; i < 5; i++) {
    unsigned char key1[50];
    unsigned char value1[50];
    sprintf(key1, "scutech%d", i);
    sprintf(value1, "dingjia%d", i);
    int keylen1 = strlen(key1);
    int valuelen1 = strlen(value1);
    // 往对象中插入数据
    begin_map(&yajlinfo);
    yajl_gen_string(yajlinfo.gen, (const unsigned char *)key1, keylen1);
    yajl_gen_string(yajlinfo.gen, (const unsigned char *)value1, valuelen1);
    end_map(&yajlinfo);
  }

#ifdef USE_MAP
  end_map(&yajlinfo);
#else
  end_array(&yajlinfo);
#endif
  // 这里是获取已经生成好的json语句，保存在buf中，长度保存在buflen中
  yajl_gen_get_buf(yajlinfo.gen, &buf, &buflen);
  // printf("%s\n", buf);
  yajl_free(&yajlinfo);


  return 0;
}