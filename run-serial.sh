#!/bin/bash

cd build
rm -rf ./*
gcc -g -w -c ../linked-list-serial.c
gcc -g -w -c ../linked-list.c
gcc -g -w -o linked-list-serial linked-list-serial.o linked-list.o -lm
cd ../ && ./build/linked-list-serial
