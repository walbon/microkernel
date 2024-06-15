#!/bin/bash
set -e
arch="${1:-mpu}"

BASE=/root/riscv-matrix-extension-spec/demos/
export PATH=${BASE}/gcc/bin:$PATH

pushd /mnt/blis/ || exit 1 
	/mnt/microkernel/Makefile.debug."${arch}"
	make clean 
	make -j10 
	make install 

	pushd /mnt/microkernel/ || exit 2
		./qemulado.sh -g 5000 & 
		./gdbado.mpu.sh
		#riscv64-unknown-linux-gnu-gdb microkernel.elf -ex "target remote localhost:5000"
	popd || exit 3
popd || exit 4
