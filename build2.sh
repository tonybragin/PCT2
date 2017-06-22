#!/bin/sh

mpicc midpoint.c -o midpoint
mpicc mcmeth.c -o mcmeth
mpicc matrx.c -o matrx
clang size.c -o size
