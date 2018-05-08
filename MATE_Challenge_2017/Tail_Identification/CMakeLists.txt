cmake_minimum_required(VERSION 2.8)
#project ( TailId_GenData )
#find_package( OpenCV REQUIRED )
#add_executable( TailId_GenData GenData.cpp )
#target_link_libraries( TailId_GenData ${OpenCV_LIBS} )

# TailID_Live
project ( TailId_Live )
include_directories("opencv")
add_executable( TailId_Live KNN_ImageRecognition.cpp )
target_link_libraries( TailId_Live ${OpenCV_LIBS} )


#[[
// To create the executable

cd <TailIdentification_directory>
cmake .
make

]]