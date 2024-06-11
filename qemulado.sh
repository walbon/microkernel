#!/bin/bash

function handle_error() {
    echo "An error occurred. Exiting of $0..."
    exit 1
}

trap handle_error ERR

set -e

qemu="/root/riscv-matrix-extension-spec/demos/qemu/bin/qemu-riscv64"

${qemu} $* -cpu rv64,x-v=true,vext_spec=v1.0,vlen=1024,x-matrix=on,rlen=256 ./microkernel.elf

exit 0
