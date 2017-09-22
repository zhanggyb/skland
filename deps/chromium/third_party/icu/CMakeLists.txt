
set(CHROMIUM_THIRD_PARTY_ICU_SOURCE_DIR "${CHROMIUM_SOURCE_DIR}/third_party/icu")

file(GLOB ICUUC_HEADERS "${CHROMIUM_THIRD_PARTY_ICU_SOURCE_DIR}/source/common/*.h")
file(GLOB ICUUC_SOURCES "${CHROMIUM_THIRD_PARTY_ICU_SOURCE_DIR}/source/common/*.cpp")
file(GLOB ICUUC_PUBLIC_HEADERS "${CHROMIUM_THIRD_PARTY_ICU_SOURCE_DIR}/source/common/unicode/*.h")

file(GLOB ICUI18N_HEADERS "${CHROMIUM_THIRD_PARTY_ICU_SOURCE_DIR}/source/i18n/*.h")
file(GLOB ICUI18N_SOURCES "${CHROMIUM_THIRD_PARTY_ICU_SOURCE_DIR}/source/i18n/*.cpp")
file(GLOB ICUI18N_PUBLIC_HEADERS "${CHROMIUM_THIRD_PARTY_ICU_SOURCE_DIR}/source/i18n/unicode/*.h")

add_definitions(-DU_USING_ICU_NAMESPACE=0)
add_definitions(-DU_ENABLE_DYLOAD=0)
add_definitions(-DU_COMMON_IMPLEMENTATION)
add_definitions(-DU_STATIC_IMPLEMENTATION)
add_definitions(-DICU_UTIL_DATA_IMPL=ICU_UTIL_DATA_STATIC)
add_definitions(-DUCHAR_TYPE=char16_t)
add_definitions(-DU_I18N_IMPLEMENTATION)

add_library(
        chromium_third_party_icu_common
        ${ICUUC_HEADERS}
        ${ICUUC_SOURCES}
        ${ICUUC_PUBLIC_HEADERS}
)

target_compile_options(
        chromium_third_party_icu_common
        PRIVATE "-Wno-unused-function"
        PRIVATE "-Wno-deprecated-declarations"
        PRIVATE "-Wno-unused-but-set-variable"
)

target_include_directories(
        chromium_third_party_icu_common
        BEFORE
        PRIVATE "${CHROMIUM_THIRD_PARTY_ICU_SOURCE_DIR}/source/common/"
)

add_library(
        chromium_third_party_icu_i18n
        ${ICUI18N_HEADERS}
        ${ICUI18N_SOURCES}
        ${ICUI18N_PUBLIC_HEADERS}
)

target_compile_options(
        chromium_third_party_icu_i18n
        PRIVATE "-Wno-misleading-indentation"
)

target_include_directories(
        chromium_third_party_icu_i18n
        BEFORE
        PRIVATE "${CHROMIUM_THIRD_PARTY_ICU_SOURCE_DIR}/source/common/"
        PRIVATE "${CHROMIUM_THIRD_PARTY_ICU_SOURCE_DIR}/source/i18n/"
)
