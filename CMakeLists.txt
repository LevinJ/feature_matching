cmake_minimum_required (VERSION 2.6)

project (featurematcher)
set(CMAKE_BUILD_TYPE Debug)


find_package( OpenCV REQUIRED )

add_executable(main main.cpp  FeatureMatchUtil.cpp DebugUtil.cpp OrbDebugUtil.cpp)

target_link_libraries(main ${OpenCV_LIBS} )
