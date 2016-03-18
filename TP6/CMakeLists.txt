cmake_minimum_required (VERSION 2.6)
project (geonum_TP6)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_definitions(
  -DTW_STATIC
  -DTW_NO_LIB_PRAGMA
  -DTW_NO_DIRECT3D
  -DGLEW_STATIC
  -D_CRT_SECURE_NO_WARNINGS
)

### OPENGL ###
find_package(OpenGL REQUIRED)

### GLFW ###
add_subdirectory (../glfw-3.1.2 "${CMAKE_CURRENT_BINARY_DIR}/glfw")

### GLEW ###
set(GLEW_SOURCE ../glew-1.13.0/src/glew.c)
set(GLEW_HEADERS)
add_library( GLEW_1130 STATIC
  ${GLEW_SOURCE}
  ${GLEW_INCLUDE}
)
target_link_libraries(GLEW_1130
  ${OPENGL_LIBRARY}
  ${EXTRA_LIBS}
)

if(UNIX AND NOT APPLE)
    set(OPENGL_LIBRARY
    ${OPENGL_LIBRARY}
    -lGL -lGLU -lXrandr -lXext -lX11 -lrt
    ${CMAKE_DL_LIBS}
    ${GLFW_LIBRARIES}
    )
elseif(APPLE)
    set(OPENGL_LIBRARY
    ${OPENGL_LIBRARY}
    -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    ${CMAKE_DL_LIBS}
    ${GLFW_LIBRARIES}
    )
elseif(DARWIN)
    set(OPENGL_LIBRARY
    ${OPENGL_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${GLFW_LIBRARIES}
    )
endif(UNIX AND NOT APPLE)

include_directories(
  .
  ..
  ../eigen-3.2.7/
  ../SimpleViewer/
  ../glfw-3.1.2/include/GLFW/
  ../glm-0.9.7.1/
  ../glew-1.13.0/include/
)

set(ALL_LIBS
  ${OPENGL_LIBRARY}
  glfw
  GLEW_1130
)

add_executable(
  ${PROJECT_NAME}
  main.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  ${ALL_LIBS}
)
