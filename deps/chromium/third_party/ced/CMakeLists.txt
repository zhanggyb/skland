# deps/chromium/third_party/ced

set(CHROMIUM_THIRD_PARTY_CED_SOURCE_DIR "${CHROMIUM_SOURCE_DIR}/third_party/ced")

add_library(chromium_third_party_ced
        ${CHROMIUM_THIRD_PARTY_CED_SOURCE_DIR}/src/compact_enc_det/compact_enc_det.cc
        ${CHROMIUM_THIRD_PARTY_CED_SOURCE_DIR}/src/compact_enc_det/compact_enc_det_hint_code.cc
        ${CHROMIUM_THIRD_PARTY_CED_SOURCE_DIR}/src/util/encodings/encodings.cc
        ${CHROMIUM_THIRD_PARTY_CED_SOURCE_DIR}/src/util/languages/languages.cc
        )
target_include_directories(
        chromium_third_party_ced
        PRIVATE "${CHROMIUM_THIRD_PARTY_CED_SOURCE_DIR}/src"
)
target_compile_options(
        chromium_third_party_ced
        PRIVATE "-Wno-narrowing"
        PRIVATE "-Wno-sign-compare"
        PRIVATE "-Wno-unused-variable"
        PRIVATE "-Wno-unused-but-set-variable"
)