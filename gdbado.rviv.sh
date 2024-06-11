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

rm -rf debug_rv64iv_K${K_size}.log microkernel.elf

echo " ############### Building the microkernel.c with K=${K_size}. ###########"
./build.sh

cat <<EOF> rv64iv.gdbinit
set pagination off
set logging file debug_rv64iv_K${K_size}.log
set logging on

target remote localhost:5000

printf "\n################ Break 1: entering of sgemm ############################\n"
break microkernel.c:61
printf "__________________________________________ Showing the entering of sgemm\n"
c
info breakpoints
printf " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> M=%i | N=%i | K=%i\n", M, N, K

printf "m\n############## Break 2: on begginng of loading of Matrix ##############\n"
break bli_sdgemm_rviv_asm_4vx4.h:189
info breakpoints
command
	printf " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> k=%i | rs_c=%i | cs_c=%i\n", \$a0, \$a6, \$a7
end

printf "\n################ Break 3: after sgemm execution ########################\n"
break microkernel.c:70
info breakpoints

printf "\n################# Show all breakpoints and counters of hints\n"
#info breakpoints

c
#i b
printf "\n________________ ignore the break 2 for 20 times\n"
printf "\n ... Executing: ignore 2 20\n"
ignore 2 20
c
#i b
printf "\n________________ ignore the break 2 for 100 times\n"
printf "\n ... Executing: ignore 2 100\n"
ignore 2 100
c
#i b
printf "\n________________ ignore the break 2 for $((K_size-5-20-100)) times\n"
printf "\n ... Executing: ignore 2 $((K_size-2))\n"
ignore 2 $((K_size-5-20-100))
c
#i b
c

set logging off
quit
EOF



./qemulado.sh -g 5000 & \
$GDB  ./microkernel.elf -ex "source rv64iv.gdbinit"

echo "DONE"
