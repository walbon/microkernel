#!/bin/bash

function handle_error() {
    echo "An error occurred. Exiting of $0..."
	kill -9 $(pidof qemu-riscv64) &> /dev/null
    exit 1
}

trap handle_error ERR

set -e

BASE=/root/riscv-matrix-extension-spec/demos/
GDB=${BASE}/gcc/bin/riscv64-unknown-linux-gnu-gdb
export PATH=${BASE}/qemu/bin:$PATH
K_size="$(grep "define K" microkernel.c |  awk -F'[^0-9]*' '{print $2}')"

rm -rf debug_mpu_K${K_size}.log microkernel.elf

echo " ############### Building the microkernel.c with K=${K_size}. ###########"
./build.sh

cat <<EOF> mpu.gdbinit
set pagination off
set logging file debug_mpu_K${K_size}.log
set logging on

target remote localhost:5000

break bli_sgemm_mpu.c:138
command
    p/x L
    bt 3
end
c
set logging off
quit
EOF



./qemulado.sh -g 5000 & \
$GDB  ./microkernel.elf -ex "source mpu.gdbinit"

echo "DONE"
exit 0

1       breakpoint     keep y   0x00000000000206f8 in main at microkernel.c:61
        breakpoint already hit 1 time
2       breakpoint     keep y   0x0000000000020744 in main at microkernel.c:70
3       breakpoint     keep y   <MULTIPLE>         
        print "Alpha=%d", (float *)alpha
3.1                         y   0x00000000000467e0 in load_alpha at kernels/mpu/3/bli_sgemm_mpu.c:70
3.2                         y   0x0000000000046980 in load_alpha at kernels/mpu/3/bli_sgemm_mpu.c:70
4       breakpoint     keep y   <MULTIPLE>         
        breakpoint already hit 1 time
        print "Beta=%d", *beta
	printf "b=%i" , (float *)b@entry
4.1                         y   0x0000000000046860 in load_beta at kernels/mpu/3/bli_sgemm_mpu.c:96
4.2                         y   0x0000000000046926 in load_beta at kernels/mpu/3/bli_sgemm_mpu.c:96
5       breakpoint     keep y   0x000000000004691c in bli_sgemm_mpu at kernels/mpu/3/bli_sgemm_mpu.c:161
        breakpoint already hit 1 time
        print "L=%i, rsi=%x", L, rsi
6       breakpoint     keep y   0x000000000004691c in bli_sgemm_mpu at kernels/mpu/3/bli_sgemm_mpu.c:161
        breakpoint already hit 1 time
7       breakpoint     keep y   0x0000000000046970 in bli_sgemm_mpu at kernels/mpu/3/bli_sgemm_mpu.c:170
        breakpoint already hit 1 time
        printf "rs_c=%i, Caddr=%x", rs_c, c
8       breakpoint     keep y   0x00000000000469e6 in bli_sgemm_mpu at kernels/mpu/3/bli_sgemm_mpu.c:194
        printf "cs_c=%i, Caddr=%d", ss_c, *c






