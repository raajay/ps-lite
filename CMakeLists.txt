cmake_minimum_required(VERSION 2.8.7)

project(pslite C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(deps_INCLUDE_DIR_L "")
list(APPEND deps_INCLUDE_DIR_L "${PROJECT_SOURCE_DIR}/deps/include/")
list(APPEND deps_INCLUDE_DIR_L "${PROJECT_SOURCE_DIR}/include/")

set(deps_LIB_DIR_L "")
list(APPEND deps_LIB_DIR_L "${PROJECT_SOURCE_DIR}/deps/lib/")
#list(APPEND deps_LIB_DIR_L "${PROJECT_SOURCE_DIR}/build/")

# ---[ Google-protobuf
include(cmake/ProtoBuf.cmake)

# generate protobuf sources
set(proto_gen_folder "${PROJECT_BINARY_DIR}/src")
file(GLOB_RECURSE proto_files "src/*.proto")
pslite_protobuf_generate_cpp_py(${proto_gen_folder} proto_srcs proto_hdrs proto_python "${PROJECT_SOURCE_DIR}" "src" ${proto_files})
include_directories(pslite "${PROJECT_SOURCE_DIR}/include/")
include_directories(pslite "${PROJECT_SOURCE_DIR}/deps/include/")
include_directories(pslite "${PROJECT_BINARY_DIR}/include/")
include_directories(pslite "${PROJECT_BINARY_DIR}/src/")

set(pslite_INCLUDE_DIR_L "")
list(APPEND pslite_INCLUDE_DIR_L "${PROJECT_BINARY_DIR}/include/")

FILE(GLOB SOURCE "src/*.cc")
list(APPEND SOURCE ${proto_srcs})

# -- specify the dependencies for pslite
add_library(pslite ${SOURCE})


# -- specify dependencies for application
add_executable(psapp app/main.cc)
add_dependencies(psapp pslite)
target_link_libraries(psapp
        "${PROJECT_SOURCE_DIR}/deps/lib/libprotobuf-lite.dylib"
        "${PROJECT_SOURCE_DIR}/deps/lib/libzmq.dylib"
        "${PROJECT_BINARY_DIR}/libpslite.a"
        )



