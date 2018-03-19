cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

OPTION(DEFINE_DEBUG
  "Build the project using debugging code"
  OFF)
IF(DEFINE_DEBUG)
  MESSAGE("Adding Debug flag...")
  ADD_DEFINITIONS(-DDEFINE_DEBUG)
  #SET(CMAKE_BUILD_TYPE Debug)
  MESSAGE("Build type is " ${CMAKE_BUILD_TYPE})
ENDIF(DEFINE_DEBUG)


set(SHORT_NAME Edge)
project(${SHORT_NAME})
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Edge")
set(VITA_TITLEID  "VEDG00223")
set(VITA_VERSION  "01.92")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=gnu11 -Wall ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall ")


include_directories(
  ../common
)

add_executable(${SHORT_NAME}.elf
  src/main.c
)

target_link_libraries(${SHORT_NAME}.elf


  debugnet
  SDL2
  vita2d
  ScePgf_stub
  ScePvf_stub
  SceGxm_stub
  SceCtrl_stub
  SceTouch_stub
  SceAppUtil_stub
  SceDisplay_stub
  SceLibKernel_stub
  SceSysmodule_stub
  ScePower_stub
  SceCommonDialog_stub
  SceRegistryMgr_stub
  SceHid_stub
  freetype
  png
  jpeg
  z
  m
  SceAudio_stub
)


vita_create_self(${SHORT_NAME}.self ${SHORT_NAME}.elf)

vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} ${SHORT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
)
