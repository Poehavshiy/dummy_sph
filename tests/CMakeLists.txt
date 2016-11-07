
 include_directories("/home/nikita/ClionProjects/dummy_sph/googletest/include")
 add_executable(testing test.cpp)

 target_link_libraries(testing gtest gtest_main)
