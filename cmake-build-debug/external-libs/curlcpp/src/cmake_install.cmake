# Install script for directory: /home/apoorva64/SecTrans2/external-libs/curlcpp/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/apoorva64/SecTrans2/cmake-build-debug/external-libs/curlcpp/src/libcurlcpp.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/curlcpp" TYPE FILE FILES
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/cookie.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/cookie_date.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/cookie_datetime.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/cookie_time.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_config.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_cookie.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_easy.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_easy_info.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_exception.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_form.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_global.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_header.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_info.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_interface.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_ios.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_multi.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_option.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_pair.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_receiver.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_sender.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_share.h"
    "/home/apoorva64/SecTrans2/external-libs/curlcpp/src/../include/curl_utility.h"
    )
endif()

