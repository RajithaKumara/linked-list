#!/bin/bash

rm -f ./build/linked-list-rwlock
gcc -g -w -o build/linked-list-rwlock linked-list-rwlock.c -lpthread
./build/linked-list-rwlock
