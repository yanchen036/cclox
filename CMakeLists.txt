cmake_minimum_required(VERSION 3.15)

project(cclox)

include_directories(SYSTEM /usr/local/Cellar/googletest/1.11.0/include)
link_directories(/usr/local/Cellar/googletest/1.11.0/lib)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(src SRCS)
aux_source_directory(autogen AUTOGEN)

add_executable(${PROJECT_NAME} ${SRCS} ${AUTOGEN})

# target_link_libraries(${PROJECT_NAME} glog gflags)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/autogen)

# tests
set(TEST_LIBS gtest gtest_main)
set(TEST_SRCS src/interpreter.cc src/parser.cc src/scanner.cc src/environment.cc autogen/expr.cc autogen/stmt.cc)

add_executable(ScannerTest test/scanner_test.cc src/scanner.cc)
target_link_libraries(ScannerTest ${TEST_LIBS})
target_include_directories(ScannerTest PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/autogen)

add_executable(ParserTest test/parser_test.cc src/parser.cc src/scanner.cc autogen/expr.cc autogen/stmt.cc)
target_link_libraries(ParserTest ${TEST_LIBS})
target_include_directories(ParserTest PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/autogen)

add_executable(InterpreterTest test/interpreter_test.cc ${TEST_SRCS})
target_link_libraries(InterpreterTest ${TEST_LIBS})
target_include_directories(InterpreterTest PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/autogen)

add_test(scanner_test ScannerTest)
add_test(parser_test ParserTest)
add_test(interpreter_test InterpreterTest)
enable_testing()
