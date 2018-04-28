cmake_minihum_required(VERSION 2.8)
project ( TailId_GenData )
find_package( OpenCV REQUIRED )
add_executable( TailId_GenData GenData.cpp )
target_link_libraries( TailId_GenData ${OpenCV_LIBS} )

cmake_minihum_required(VERSION 2.8)
project ( TailId_Live )
find_package( OpenCV REQUIRED )
add_executable( TailId_Live KNN_ImageRecognition.cpp )
target_link_libraries( TailId_Live ${OpenCV_LIBS} )

#[[
// To create the executable

cd <TailIdentification_directory>
cmake .
make

]]