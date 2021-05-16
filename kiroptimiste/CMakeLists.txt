cmake_minimum_required(VERSION 2.6)
list(APPEND CMAKE_MODULE_PATH "${p}") #For old Imagine++
list(APPEND CMAKE_SYSTEM_FRAMEWORK_PATH /Library/Frameworks) #Mac, why not auto?
find_package(Imagine)
project(KKKiroptimiste)
add_executable(KKKiroptimiste KKKiroptimiste.cpp annexe.cpp annexe.h)
ImagineUseModules(KKKiroptimiste Graphics)
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
