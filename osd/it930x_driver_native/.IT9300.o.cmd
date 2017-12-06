cmd_/home/ubuntu/VacronControl/osd/it930x_driver_native/IT9300.o := gcc -Wp,-MD,/home/ubuntu/VacronControl/osd/it930x_driver_native/.IT9300.o.d  -nostdinc -isystem /usr/lib/gcc/x86_64-linux-gnu/4.8/include  -I./arch/x86/include -Iarch/x86/include/generated/uapi -Iarch/x86/include/generated  -Iinclude -I./arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -Iubuntu/include  -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -m64 -falign-jumps=1 -falign-loops=1 -mno-80387 -mno-fp-ret-in-387 -mtune=generic -mno-red-zone -mcmodel=kernel -funit-at-a-time -maccumulate-outgoing-args -DCONFIG_X86_X32_ABI -DCONFIG_AS_FXSAVEQ=1 -DCONFIG_AS_SSSE3=1 -DCONFIG_AS_CRC32=1 -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -fno-delete-null-pointer-checks -O2 --param=allow-store-data-races=0 -Wframe-larger-than=1024 -fstack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -pg -mfentry -DCC_USING_FENTRY -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -DCC_HAVE_ASM_GOTO -DEXPORT_SYMTAB -Wno-unused-value -Wno-unused-variable -Wno-unused-parameter -Wno-switch -O2 -I..  -DMODULE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(IT9300)"  -D"KBUILD_MODNAME=KBUILD_STR(usb_it930x)" -c -o /home/ubuntu/VacronControl/osd/it930x_driver_native/.tmp_IT9300.o /home/ubuntu/VacronControl/osd/it930x_driver_native/IT9300.c

source_/home/ubuntu/VacronControl/osd/it930x_driver_native/IT9300.o := /home/ubuntu/VacronControl/osd/it930x_driver_native/IT9300.c

deps_/home/ubuntu/VacronControl/osd/it930x_driver_native/IT9300.o := \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/IT9300.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/brType.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/type.h \
    $(wildcard include/config/compat.h) \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/userdef.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/inttype.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/user.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/error.h \
    $(wildcard include/config/header.h) \
    $(wildcard include/config/size.h) \
    $(wildcard include/config/sn.h) \
    $(wildcard include/config/subtype.h) \
    $(wildcard include/config/value.h) \
    $(wildcard include/config/chksum.h) \
    $(wildcard include/config/value.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include/uapi/linux/types.h \
  arch/x86/include/uapi/asm/types.h \
  include/uapi/asm-generic/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  arch/x86/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/gcov/kernel.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  arch/x86/include/asm/posix_types.h \
    $(wildcard include/config/x86/32.h) \
  arch/x86/include/uapi/asm/posix_types_64.h \
  include/uapi/asm-generic/posix_types.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/brUser.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/brError.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/brRegister.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/brVariable.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/brVersion.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/brCmd.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/usb2impl.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/IT9133.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/register.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/variable.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/cmd.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/standard.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/version.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/brfirmware.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/brTiming.h \

/home/ubuntu/VacronControl/osd/it930x_driver_native/IT9300.o: $(deps_/home/ubuntu/VacronControl/osd/it930x_driver_native/IT9300.o)

$(deps_/home/ubuntu/VacronControl/osd/it930x_driver_native/IT9300.o):
