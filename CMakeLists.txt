cmake_minimum_required (VERSION 2.6)

project (featurematcher)
set(CMAKE_BUILD_TYPE Debug)
set(OpenCV_DIR  /home/aiways/anaconda3/share/OpenCV)
find_package( OpenCV REQUIRED )

add_executable(main featurematcher.cpp GdbTool.cpp)
target_link_libraries(main ${OpenCV_LIBS} )
