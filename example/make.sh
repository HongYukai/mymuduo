<<<<<<< HEAD
#!/bin/bash

for var in $(seq 8180 8180);
do
  ./testHttpServer "$var" &    # command a & command b & command c: 三个命令同时执行
done
=======
#!/bin/sh

set -e

processes=$( ps -ux | grep testHttpServer | grep $2 | tr \t ' ' | tr -s ' ' | cut -d ' ' -f 2 | uniq )
for process in ${processes}
do
        kill ${process}
done

for var in $(seq 1 $1)
do
  ./testHttpServer $2 & # command a & command b & command c: 三个命令同时执行
done
echo Create $1 hongws processes successfully.
>>>>>>> 8cdbf3f71c984f3a6a3726ff79e7cba1b1ad1ad4
