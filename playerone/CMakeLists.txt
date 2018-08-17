set(TARGET "playerone")
add_executable(${TARGET}.wasm ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET}.cpp)

target_include_directories(${TARGET}.wasm
   PUBLIC 
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(${TARGET}.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/abi/${TARGET}.abi" "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)