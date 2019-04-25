{
  "targets": [
    {
      "target_name": "cryptonight-old-hardware",
      "sources": [
        '<!@(uname -a | grep "x86_64" >/dev/null && echo "lib/vendor/crypto/asm/cn_main_loop.S" || echo)',
        '<!@(uname -a | grep "x86_64" >/dev/null && echo "lib/vendor/crypto/asm/CryptonightR_template.S" || echo)',
        '<!@(uname -a | grep "x86_64" >/dev/null && echo "lib/vendor/crypto/CryptonightR_gen.cpp" || echo)',
        '<!@(uname -a | grep "x86_64" >/dev/null && (grep avx2 /proc/cpuinfo >/dev/null && echo "lib/vendor/crypto/cn_gpu_avx.cpp" || echo) || echo)',
        '<!@(uname -a | grep "x86_64" >/dev/null && echo "lib/vendor/crypto/cn_gpu_ssse3.cpp" || echo)',
        '<!@(uname -a | grep "x86_64" >/dev/null || echo "lib/vendor/crypto/cn_gpu_arm.cpp" || echo)',
        '<!@(uname -a | grep "x86_64" >/dev/null && echo "lib/vendor/common/cpu/Cpu.cpp" || echo)',
        '<!@(uname -a | grep "x86_64" >/dev/null && echo "lib/vendor/common/cpu/BasicCpuInfo.cpp" || echo)',
        '<!@(uname -a | grep "x86_64" >/dev/null || echo "lib/vendor/common/cpu/BasicCpuInfo_arm.cpp" || echo)',
        "lib/vendor/extra.cpp",
        "lib/vendor/Mem.cpp",
        "lib/vendor/Mem_unix.cpp",
        "lib/cryptonight-old-hardware.cc",
        "lib/vendor/crypto/c_blake256.c",
        "lib/vendor/crypto/c_groestl.c",
        "lib/vendor/crypto/c_jh.c",
        "lib/vendor/crypto/c_skein.c",
        "lib/vendor/crypto/CryptonightR_gen.cpp",
        "lib/vendor/common/crypto/keccak.cpp"
      ],
      "include_dirs": [
        "lib/vendor",
        "lib/vendor/3rdparty",
        "<!(node -e \"require('nan')\")"
      ],
      "cflags_c": [
        '<!@(uname -a | grep "aarch64" >/dev/null && echo "-march=armv8-a+crypto" || (uname -a | grep "armv7" >/dev/null && echo "-mfpu=neon -flax-vector-conversions" || echo "-march=native"))',
        "-std=gnu11 -fPIC -DNDEBUG -Ofast -funroll-loops -fvariable-expansion-in-unroller -ftree-loop-if-convert-stores -fmerge-all-constants -fbranch-target-load-optimize2"
      ],
      "cflags_cc": [
        '<!@(uname -a | grep "aarch64" >/dev/null && echo "-march=armv8-a+crypto -flax-vector-conversions" || (uname -a | grep "armv7" >/dev/null && echo "-mfpu=neon -flax-vector-conversions" || echo "-march=native"))',
        "-std=gnu++11 -fPIC -DNDEBUG -Ofast -s -funroll-loops -fvariable-expansion-in-unroller -ftree-loop-if-convert-stores -fmerge-all-constants -fbranch-target-load-optimize2"
      ]
    }
  ]
}
