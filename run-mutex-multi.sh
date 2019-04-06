#!/bin/bash

cd build
rm -rf ./*
gcc -g -w -c ../linked-list-mutex-multi.c
gcc -g -w -c ../linked-list.c
gcc -g -w -o linked-list-mutex-multi linked-list-mutex-multi.o linked-list.o -lpthread -lm
cd ../ && ./build/linked-list-mutex-multi
