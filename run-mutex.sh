#!/bin/bash

rm -f ./build/linked-list-mutex
gcc -g -w -o build/linked-list-mutex linked-list-mutex.c -lpthread
./build/linked-list-mutex
