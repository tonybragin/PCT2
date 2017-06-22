#!/bin/sh

mpicc ring.c -o ring
mpicc broadcast.c -o broadcast
mpicc gather.c -o gather
mpicc alltoall.c -o alltoall
