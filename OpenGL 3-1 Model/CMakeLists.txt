#需求的最低cmake程序版本
cmake_minimum_required(VERSION 3.12)

#本工程的名字
project(Learn_OpenGL)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

#将素材文件夹复制到执行目录
file(GLOB ASSETS "./assets" )
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY "./libs/assimp-vc143-mt.dll" DESTINATION ${CMAKE_BINARY_DIR})

#链接/libs下的.lib文件
link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/libs
)

#将/include加入头文件目录
include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#将include添加为子目录，并将其下所有.c/.cpp文件编译
file(GLOB_RECURSE CPPS ./include/*.c *.cpp  )
add_library(cpps ${CPPS} "include/model.h")

#将本工程所有cpp文件编译链接，生成exe
add_executable(openglStudy "main.cpp")

target_link_libraries(openglStudy glfw3.lib assimp-vc143-mt.lib cpps)