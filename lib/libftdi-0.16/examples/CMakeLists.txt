# Includes
include( ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_BINARY_DIR}
         )

# Source includes
include_directories(${CMAKE_SOURCE_DIR}/src)

# Targets
add_executable(simple simple.c)
add_executable(bitbang bitbang.c)
add_executable(bitbang2 bitbang2.c)
add_executable(bitbang_cbus bitbang_cbus.c)
add_executable(bitbang_ft2232 bitbang_ft2232.c)
add_executable(find_all find_all.c)

# Linkage
target_link_libraries(simple ftdi)
target_link_libraries(bitbang ftdi)
target_link_libraries(bitbang2 ftdi)
target_link_libraries(bitbang_cbus ftdi)
target_link_libraries(bitbang_ft2232 ftdi)
target_link_libraries(find_all ftdi)

# libftdi++ examples
if(FTDI_BUILD_CPP)
   message(STATUS "Building libftdi++ examples.")
   include_directories(${CMAKE_SOURCE_DIR}/ftdipp)

   # Targest
   add_executable(find_all_pp find_all_pp.cpp)

   # Linkage
   target_link_libraries(find_all_pp ftdipp)
endif()
