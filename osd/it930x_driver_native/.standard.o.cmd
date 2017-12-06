cmd_/home/ubuntu/VacronControl/osd/it930x_driver_native/standard.o := gcc -Wp,-MD,/home/ubuntu/VacronControl/osd/it930x_driver_native/.standard.o.d  -nostdinc -isystem /usr/lib/gcc/x86_64-linux-gnu/4.8/include  -I./arch/x86/include -Iarch/x86/include/generated/uapi -Iarch/x86/include/generated  -Iinclude -I./arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I./include/uapi -Iinclude/generated/uapi -include ./include/linux/kconfig.h -Iubuntu/include  -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -m64 -falign-jumps=1 -falign-loops=1 -mno-80387 -mno-fp-ret-in-387 -mtune=generic -mno-red-zone -mcmodel=kernel -funit-at-a-time -maccumulate-outgoing-args -DCONFIG_X86_X32_ABI -DCONFIG_AS_FXSAVEQ=1 -DCONFIG_AS_SSSE3=1 -DCONFIG_AS_CRC32=1 -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -fno-delete-null-pointer-checks -O2 --param=allow-store-data-races=0 -Wframe-larger-than=1024 -fstack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -pg -mfentry -DCC_USING_FENTRY -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -DCC_HAVE_ASM_GOTO -DEXPORT_SYMTAB -Wno-unused-value -Wno-unused-variable -Wno-unused-parameter -Wno-switch -O2 -I..  -DMODULE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(standard)"  -D"KBUILD_MODNAME=KBUILD_STR(usb_it930x)" -c -o /home/ubuntu/VacronControl/osd/it930x_driver_native/.tmp_standard.o /home/ubuntu/VacronControl/osd/it930x_driver_native/standard.c

source_/home/ubuntu/VacronControl/osd/it930x_driver_native/standard.o := /home/ubuntu/VacronControl/osd/it930x_driver_native/standard.c

deps_/home/ubuntu/VacronControl/osd/it930x_driver_native/standard.o := \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/standard.h \
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
  /home/ubuntu/VacronControl/osd/it930x_driver_native/register.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/variable.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/version.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/cmd.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/tuner.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/firmware.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/firmware_V2.h \
  /home/ubuntu/VacronControl/osd/it930x_driver_native/firmware_V2I.h \

/home/ubuntu/VacronControl/osd/it930x_driver_native/standard.o: $(deps_/home/ubuntu/VacronControl/osd/it930x_driver_native/standard.o)

$(deps_/home/ubuntu/VacronControl/osd/it930x_driver_native/standard.o):
