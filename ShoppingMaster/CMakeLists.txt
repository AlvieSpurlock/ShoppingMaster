cmake_minimum_required(VERSION 3.15)
project(ShoppingMaster VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

set(SOURCES
    ShoppingApp.cpp
    ShoppingFrame.cpp
    ShoppingFrame_Layout.cpp
    ShoppingFrame_Lists.cpp
    ShoppingFrame_Handlers.cpp
    ShoppingDialogs.cpp
)

if(WIN32)
    add_executable(ShoppingMaster WIN32 ${SOURCES})
else()
    add_executable(ShoppingMaster ${SOURCES})
endif()

target_link_libraries(ShoppingMaster PRIVATE ${wxWidgets_LIBRARIES})
if(MSVC)
    set_target_properties(ShoppingMaster PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
endif()