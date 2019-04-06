#!/bin/bash

cd build
rm -rf ./*
gcc -g -w -c ../linked-list-mutex.c
gcc -g -w -c ../linked-list.c
gcc -g -w -o linked-list-mutex linked-list-mutex.o linked-list.o -lpthread -lm
cd ../ && ./build/linked-list-mutex
