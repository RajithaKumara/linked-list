#!/bin/bash

# rm -f ./test.out/linked-list-serial
# rm -f ./test.out/linked-list-mutex
# rm -f ./test.out/linked-list-rwlock
# rm -rf ./test.out/*
cd build
rm -rf ./*

gcc -g -w -c ../linked-list.c
gcc -g -w -c ../linked-list-serial.c
gcc -g -w -c ../linked-list-mutex.c
gcc -g -w -c ../linked-list-rwlock.c
# gcc -g -w -c ../linked-list-mutex-multi.c

gcc -g -w -o linked-list-serial linked-list-serial.o linked-list.o -lm
gcc -g -w -o linked-list-mutex linked-list-mutex.o linked-list.o -lpthread -lm
gcc -g -w -o linked-list-rwlock linked-list-rwlock.o linked-list.o -lpthread -lm
# gcc -g -w -o linked-list-mutex-multi linked-list-mutex-multi.o linked-list.o -lpthread -lm

cp linked-list-serial ../test.out/
cp linked-list-mutex ../test.out/
cp linked-list-rwlock ../test.out/

cd ../

for((j = 5; j <= 20; j=j*2)); do
    export LL_NUMBER=$j
    ./test/test-serial.sh
    ./test/test-mutex.sh
    ./test/test-rwlock.sh
done
