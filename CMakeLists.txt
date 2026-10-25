cmake_minimum_required(VERSION 3.20)
project(avl-rb-trees VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(APP_NAME "avl_rb_trees")

add_executable(${APP_NAME}
    main.cpp
    src/AVLNode.cpp
    src/AVLTree.cpp
    src/LLRBNode.cpp
    src/LLRBTree.cpp
)

target_include_directories(${APP_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib
)

target_compile_features(${APP_NAME} PUBLIC cxx_std_17)