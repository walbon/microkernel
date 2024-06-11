#!/bin/bash

function handle_error() {
    echo "An error occurred. Exiting of $0..."
    exit 1
}

trap handle_error ERR

set -e

BASE=/root/riscv-matrix-extension-spec/demos/
CC=${BASE}/gcc/bin/riscv64-unknown-linux-gnu-gcc
OBJDUMP=${BASE}gcc/bin/riscv64-unknown-linux-gnu-objdump
GDB=${BASE}/gcc/bin/riscv64-unknown-linux-gnu-gdb

export PATH=${BASE}/qemu/bin:$PATH

echo " ## Building microkernel.c"
$CC  -I /usr/local/include/ -L /usr/local/lib/ \
	microkernel.c -o microkernel.elf \
	-lblis -lm -static -lpthread -l:libblis.a \
 	-I/mnt/blis/include/ -L/mnt/blis/lib/ \
	-g3 -O0 $1 

echo " ## Converting elf file into OBJDUMP w/Opcodes"
$OBJDUMP -S microkernel.elf > microkernel.OBJDUMP

exit 0
