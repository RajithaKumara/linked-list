
# Linked-list

> This project is used for performance test of a parallel programming with linked list by using Pthread library.

#### Prerequisites

* `gcc`
* `pthread` library

#### Usage
Serial program
``` bash
$ ./run-serial.sh
```
Parallel program with one mutex for the entire linked list
``` bash
$ ./run-mutex.sh
```
Parallel program with read-write locks for the entire linked list
``` bash
$ ./run-rwlock.sh
```
Parallel program with multiple mutex
``` bash
$ ./run-mutex-multi.sh
```
