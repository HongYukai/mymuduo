#!/bin/bash

for var in $(seq 8180 8180);
do
  ./testHttpServer "$var" &    # command a & command b & command c: 三个命令同时执行
done