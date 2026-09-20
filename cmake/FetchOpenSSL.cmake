# FetchOpenSSL.cmake
#
# Downloads the prebuilt OpenSSL package produced by the build-openssl.yml
# workflow (attached to a GitHub Release) for the platform being configured,
# and points FindOpenSSL at it.
#
# Usage:
#   list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#   include(FetchOpenSSL)
#   fetch_openssl(
#     REPO         name/repo              # repo that holds the Release
#     TAG          v1.0                   # release tag containing the packages
#     OPENSSL_REF  openssl-3.5.0          # must match the workflow's OPENSSL_REF
#     [STATIC]                            # link libssl.a / libcrypto.a
#   )
#   find_package(OpenSSL REQUIRED)
#   target_link_libraries(myapp PRIVATE OpenSSL::SSL OpenSSL::Crypto)
#   openssl_post_link(myapp)              # DLL copy / rpath for shared builds
#
# Skips downloading if -DOPENSSL_ROOT_DIR=... is present.
# Requires CMake 3.18+.

include_guard(GLOBAL)
include(FetchContent)

function(fetch_openssl)
    cmake_parse_arguments(ARG "STATIC" "REPO;TAG;OPENSSL_REF;SHA256" "" ${ARGN})

    foreach(required REPO TAG OPENSSL_REF)
        if(NOT ARG_${required})
            message(FATAL_ERROR "fetch_openssl: missing required argument ${required}")
        endif()
    endforeach()

    # Let the user override with a local OpenSSL (e.g. -DOPENSSL_ROOT_DIR=/opt/openssl)
    if(OPENSSL_ROOT_DIR)
        message(STATUS "fetch_openssl: using user-provided OPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}")
        return()
    endif()

    # --- Map CMake's view of the platform to the workflow's artifact names ---
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(os "linux")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(os "macos")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(os "windows")
    else()
        message(FATAL_ERROR "fetch_openssl: no prebuilt package for ${CMAKE_SYSTEM_NAME}")
    endif()

    set(proc "${CMAKE_SYSTEM_PROCESSOR}")
    if(APPLE AND CMAKE_OSX_ARCHITECTURES)
        list(GET CMAKE_OSX_ARCHITECTURES 0 proc)   # universal builds: not supported here
    endif()
    string(TOLOWER "${proc}" proc)

    if(proc MATCHES "^(x86_64|amd64)$")
        set(arch "x86_64")
    elseif(proc MATCHES "^(aarch64|arm64)$")
        set(arch "arm64")
    else()
        message(FATAL_ERROR "fetch_openssl: unsupported CPU architecture '${proc}'")
    endif()

    if(os STREQUAL "windows")
        if(NOT arch STREQUAL "x86_64")
            message(FATAL_ERROR "fetch_openssl: only windows-x64 is built by the workflow")
        endif()
        set(platform "windows-x64")
        set(ext "zip")
    else()
        set(platform "${os}-${arch}")
        set(ext "tar.gz")
    endif()

    set(asset "openssl-${ARG_OPENSSL_REF}-${platform}.${ext}")
    set(url "https://github.com/${ARG_REPO}/releases/download/${ARG_TAG}/${asset}")

    # --- Download + extract (cached in the build tree; re-runs are no-ops) ---
    set(extra_args "")
    if(ARG_SHA256)
        list(APPEND extra_args URL_HASH SHA256=${ARG_SHA256})
    endif()

    message(STATUS "fetch_openssl: ${url}")
    FetchContent_Declare(openssl_prebuilt
            URL "${url}"
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
            ${extra_args}
    )
    FetchContent_MakeAvailable(openssl_prebuilt)   # no CMakeLists.txt inside: just populates

    # Sanity check
    if(NOT EXISTS "${openssl_prebuilt_SOURCE_DIR}/include/openssl/ssl.h")
        message(FATAL_ERROR
                "fetch_openssl: package layout unexpected, no include/openssl/ssl.h in "
                "${openssl_prebuilt_SOURCE_DIR}")
    endif()

    # Export to the caller so find_package(OpenSSL) picks it up.
    # FindOpenSSL searches OPENSSL_ROOT_DIR first, ahead of system locations.
    set(OPENSSL_ROOT_DIR "${openssl_prebuilt_SOURCE_DIR}" PARENT_SCOPE)
    if(ARG_STATIC)
        set(OPENSSL_USE_STATIC_LIBS ON PARENT_SCOPE)
    endif()
endfunction()


# Call after add_executable/add_library. For shared builds this makes the app
# runnable straight from the build directory:
#   - Windows: copies libssl/libcrypto DLLs next to the target
#   - Linux:   adds the OpenSSL lib dir to the build RPATH
# (macOS shared builds have CI-baked install names; prefer STATIC there.)
function(openssl_post_link target)
    if(OPENSSL_USE_STATIC_LIBS OR NOT OPENSSL_ROOT_DIR)
        return()
    endif()

    if(WIN32)
        file(GLOB dlls "${OPENSSL_ROOT_DIR}/bin/*.dll")
        if(dlls)
            add_custom_command(TARGET ${target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${dlls} "$<TARGET_FILE_DIR:${target}>"
                    COMMENT "Copying OpenSSL DLLs")
        endif()
    elseif(UNIX AND NOT APPLE)
        set_property(TARGET ${target} APPEND PROPERTY BUILD_RPATH
                "${OPENSSL_ROOT_DIR}/lib" "${OPENSSL_ROOT_DIR}/lib64")
    endif()
endfunction()