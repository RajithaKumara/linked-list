#!/bin/bash

cd build
rm -rf ./*
gcc -g -w -c ../linked-list-rwlock.c
gcc -g -w -c ../linked-list.c
gcc -g -w -o linked-list-rwlock linked-list-rwlock.o linked-list.o -lpthread -lm
cd ../ && ./build/linked-list-rwlock
