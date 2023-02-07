cmake_minimum_required(VERSION 3.0.0)
project(semaphore VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(exercice-1 src/ex1/main.c src/ex1/helper.h lib/ipc.h)
include_directories(lib)

add_executable(exercice-2 src/ex2/main.c lib/ipc.h)
include_directories(lib)

add_executable(exercice-3-v1 src/ex3/main-v1.c lib/shm.h)
include_directories(lib)

add_executable(exercice-3-v2 src/ex3/main-v2.c lib/shm.h)
include_directories(lib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
