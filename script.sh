#!/bin/sh

# Запуск только в директории с исходником
g++ main.cpp -o main
for i in 1 2 4 8 16 32 64 128 256 512 1024; do
  echo "Threads: $i"
  ./main $i
done
