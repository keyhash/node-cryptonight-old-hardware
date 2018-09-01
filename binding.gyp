{
  "targets": [
    {
      "target_name": "cryptonight-old-hardware",
      "sources": [
        "lib/cryptonight-old-hardware.cc",
        "lib/vendor/crypto/c_blake256.c",
        "lib/vendor/crypto/c_groestl.c",
        "lib/vendor/crypto/c_jh.c",
        "lib/vendor/crypto/c_skein.c",
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
