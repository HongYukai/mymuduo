# 有error直接退出
set -e

cmake . && make

# 把头文件拷贝到/usr/include/mymuduo so库拷贝到/usr/lib
if [ ! -d /usr/include/mymuduo/ ]
  then
  mkdir /usr/include/mymuduo/
fi

for header in `ls *.h`
do
  cp $header /usr/include/mymuduo/
done

cp `pwd`/lib/libmymuduo.so /usr/lib

# 刷新动态库缓存
ldconfig
