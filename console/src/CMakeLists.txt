cmake_minimum_required(VERSION 3.19)
project(FilesHash)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(boost_1_78_0)

add_executable(files_hash
        main.cpp
        DirectoryTree.cpp           DirectoryTree.h
        hash.cpp                    hash.h
        Timer.cpp                   Timer.h
        common.cpp                  common.h
        ProgressBar.cpp             ProgressBar.h
        DelayedErrorsBuffer.cpp     DelayedErrorsBuffer.h
        )
