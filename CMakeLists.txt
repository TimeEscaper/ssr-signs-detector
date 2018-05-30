cmake_minimum_required(VERSION 3.10)
project(FirstProject)

set(CMAKE_CXX_STANDARD 11)
find_package( OpenCV REQUIRED )

add_executable(FirstProject src/main.cpp src/Trainer.cpp include/Trainer.h src/DescriptorFactory.cpp include/DescriptorFactory.h include/ImageConstants.h src/DataSetImage.cpp include/DataSetImage.h src/DataSetSign.cpp include/DataSetSign.h src/DataSet.cpp include/DataSet.h src/SignDetector.cpp include/SignDetector.h src/Util.cpp include/Util.h)

include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( FirstProject ${OpenCV_LIBS} )
