# From d-SEAMS : https://github.com/d-seams/seams-core
# --- Externals
include(../cmake/CPM.cmake)
CPMAddPackage("gh:catchorg/Catch2@2.13.4")

add_executable(particle_tests
               main.cpp
               vector_particle-test.cpp
               phys_processes-test.cpp
)
 
# Link everything
target_link_libraries(particle_tests
  ioplib
  Catch2
  )
target_compile_features(particle_tests PRIVATE cxx_std_17)

# Project Libraries
include_directories(
  ${PROJECT_SOURCE_DIR}/src/include/data_types
  ${PROJECT_SOURCE_DIR}/src/include/
  )

# ---- Run tests ----
add_test(NAME particleIOP-unit-tests COMMAND $<TARGET_FILE:particle_tests>)
