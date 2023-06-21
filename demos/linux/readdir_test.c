#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

using namespace std;


bool BlDirDelete(const string& directory)
{
  char cur_dir[] = ".";
  char up_dir[] = "..";
  DIR *dirp = NULL;
  struct dirent *dp = NULL;
  struct stat dir_stat;
  const char *dir = directory.c_str();

  // 获取目录属性失败，返回错误
  if (0 > stat(dir, &dir_stat)) {
    printf("get directory stat error:%s\n", dir);
    return false;
  }
//
//  if (S_ISREG(dir_stat.st_mode)) {	// 普通文件直接删除
//    if (0 != remove(dir))
//    {
//      printf("remove file %s failed with error %d\n", dir, errno);
//      return false;
//    }
//    return true;
//  }
	
  if (S_ISDIR(dir_stat.st_mode)) {	// 目录文件，递归删除目录中内容
    printf("openddir b\n");
    sleep(5);
    dirp = opendir(dir);
    printf("openddir a\n");
//    auto my_readdir = [&](DIR *d){
//      std::unique_lock<std::mutex> lck(g_dir_mux);
//      return readdir(d);
//    };
//    dirp = nullptr;
    sleep(10);
    while ((dp = readdir(dirp)) != NULL) {
      // 忽略 . 和 ..
      if ((0 == strcmp(cur_dir, dp->d_name)) || (0 == strcmp(up_dir, dp->d_name))) {
        continue;
      }
      string dir_name = string(dir) + "/" + string(dp->d_name);
      BlDirDelete(dir_name);   // 递归调用
    }
    closedir(dirp);

    if (0 != rmdir(dir)) // 删除空目录
    {
      printf("remove dir %s failed with error %d\n", dir, errno);
      return false;
    }
    return true;
  }
  else {
    printf("unknown file type!\n");
    return false;
  }

  return true;
}

int main(int argc, char **argv) {
  auto dir = argv[1];
  BlDirDelete(dir);

  return 0;
}