#!/bin/bash
# Build script for CPUTwo Minix microkernel
set -e

ROOT=/Users/john/minix-CPUTwo
CC=$ROOT/../cputwo-netbsd-toolchain/bin/cputwo--netbsd-clang
LD=$ROOT/../cputwo-netbsd-toolchain/bin/cputwo--netbsd-ld
OBJDIR=$ROOT/obj.cputwo/kernel

mkdir -p $OBJDIR

CFLAGS=(
    -c -ffreestanding -nostdinc
    -D__minix -D_KERNEL -D__kernel__ -D_MINIX_SYSTEM
    -DCONFIG_MAX_CPUS=1 -D__cputwo__
    -Wno-macro-redefined
    -fno-stack-protector
)

INCLUDES=(
    -I $ROOT/minix
    -I $ROOT/minix/kernel
    -I $ROOT/minix/kernel/arch/cputwo
    -I $ROOT/minix/kernel/arch/cputwo/include
    -I $ROOT/minix/include
    -I $ROOT/minix/include/arch/cputwo/include
    -I $ROOT/sys
    -I $ROOT/sys/arch/cputwo/include
    -I $ROOT/include
    -I $ROOT/common/include
)

# Arch-specific C files
ARCH_SRCS=(
    arch_clock.c arch_do_vmctl.c arch_system.c
    exception.c hw_intr.c memory.c
    protect.c direct_tty_utils.c arch_reset.c
    pg_utils.c pre_init.c
)

# Generic kernel C files
KERN_SRCS=(
    main.c proc.c clock.c interrupt.c system.c
    table.c utility.c cpulocals.c usermapped_data.c
)

# Kernel system/ subdirectory files
SYSTEM_SRCS=()
for f in $ROOT/minix/kernel/system/*.c; do
    SYSTEM_SRCS+=($(basename $f))
done

# Assembly files
ARCH_ASM=(
    head.S mpx.S klib.S
)

# C library stubs needed by kernel
LIBSTUB_SRCS=(
    $ROOT/common/lib/libc/string/memcpy.c
    $ROOT/common/lib/libc/string/memmove.c
    $ROOT/common/lib/libc/string/memset.c
    $ROOT/common/lib/libc/string/strlen.c
    $ROOT/common/lib/libc/string/strcmp.c
    $ROOT/common/lib/libc/string/strcpy.c
    $ROOT/common/lib/libc/string/strncmp.c
    $ROOT/common/lib/libc/string/strncpy.c
    $ROOT/common/lib/libc/string/strcat.c
    $ROOT/sys/lib/libsa/printf.c
    $ROOT/sys/lib/libsa/subr_prf.c
    $ROOT/minix/lib/libminc/atoi.c
    $ROOT/minix/lib/libsys/stacktrace.c
)

OBJS=()
ERRORS=0

echo "=== Compiling assembly files ==="
for f in "${ARCH_ASM[@]}"; do
    src=$ROOT/minix/kernel/arch/cputwo/$f
    obj=$OBJDIR/${f%.S}.o
    echo -n "  $f: "
    if $CC "${CFLAGS[@]}" "${INCLUDES[@]}" -o $obj $src 2>/tmp/cputwo_err.txt; then
        echo "OK"
        OBJS+=($obj)
    else
        echo "FAIL"
        head -5 /tmp/cputwo_err.txt
        ERRORS=$((ERRORS + 1))
    fi
done

echo ""
echo "=== Compiling arch-specific files ==="
for f in "${ARCH_SRCS[@]}"; do
    src=$ROOT/minix/kernel/arch/cputwo/$f
    obj=$OBJDIR/${f%.c}.o
    echo -n "  $f: "
    if $CC "${CFLAGS[@]}" "${INCLUDES[@]}" -o $obj $src 2>/tmp/cputwo_err.txt; then
        echo "OK"
        OBJS+=($obj)
    else
        echo "FAIL"
        head -5 /tmp/cputwo_err.txt
        ERRORS=$((ERRORS + 1))
    fi
done

echo ""
echo "=== Compiling generic kernel files ==="
for f in "${KERN_SRCS[@]}"; do
    src=$ROOT/minix/kernel/$f
    obj=$OBJDIR/${f%.c}.o
    echo -n "  $f: "
    if $CC "${CFLAGS[@]}" "${INCLUDES[@]}" -o $obj $src 2>/tmp/cputwo_err.txt; then
        echo "OK"
        OBJS+=($obj)
    else
        echo "FAIL"
        head -5 /tmp/cputwo_err.txt
        ERRORS=$((ERRORS + 1))
    fi
done

echo ""
echo "=== Compiling kernel/system files ==="
for f in "${SYSTEM_SRCS[@]}"; do
    src=$ROOT/minix/kernel/system/$f
    obj=$OBJDIR/sys_${f%.c}.o
    echo -n "  $f: "
    if $CC "${CFLAGS[@]}" "${INCLUDES[@]}" -o $obj $src 2>/tmp/cputwo_err.txt; then
        echo "OK"
        OBJS+=($obj)
    else
        echo "FAIL"
        head -3 /tmp/cputwo_err.txt
        ERRORS=$((ERRORS + 1))
    fi
done

echo ""
echo "=== Compiling libtimers ==="
for f in $ROOT/minix/lib/libtimers/*.c; do
    bn=$(basename $f)
    obj=$OBJDIR/${bn%.c}.o
    echo -n "  $bn: "
    if $CC "${CFLAGS[@]}" "${INCLUDES[@]}" -o $obj $f 2>/tmp/cputwo_err.txt; then
        echo "OK"
        OBJS+=($obj)
    else
        echo "FAIL"
        head -3 /tmp/cputwo_err.txt
        ERRORS=$((ERRORS + 1))
    fi
done

echo ""
echo "=== Compiling library stubs ==="
STUB_CFLAGS=("${CFLAGS[@]}" -Dputchar=kputc -DLIBSA_PRINTF_LONGLONG_SUPPORT -DLIBSA_PRINTF_WIDTH_SUPPORT -fno-builtin -D_STANDALONE -U_KERNEL -Wno-implicit-int)
for src in "${LIBSTUB_SRCS[@]}"; do
    f=$(basename $src)
    obj=$OBJDIR/${f%.c}.o
    echo -n "  $f: "
    if $CC "${STUB_CFLAGS[@]}" "${INCLUDES[@]}" -o $obj $src 2>/tmp/cputwo_err.txt; then
        echo "OK"
        OBJS+=($obj)
    else
        echo "FAIL"
        head -5 /tmp/cputwo_err.txt
        ERRORS=$((ERRORS + 1))
    fi
done

echo ""
echo "=== Results: $ERRORS errors ==="
if [ $ERRORS -eq 0 ]; then
    echo "All files compiled successfully!"
    echo ""
    echo "=== Linking kernel ==="
    $LD -nostdlib -T $ROOT/minix/kernel/arch/cputwo/kernel.lds \
        -o $OBJDIR/kernel "${OBJS[@]}" 2>&1 | head -20 || true
    echo "Kernel at: $OBJDIR/kernel"
fi
