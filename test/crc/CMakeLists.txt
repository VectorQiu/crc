file(GLOB TEST_MEMORY_POOL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(test_crc ${TEST_MEMORY_POOL_SOURCES})

target_link_libraries(test_crc PRIVATE
    GTest::GTest
    GTest::Main
    crc
)
