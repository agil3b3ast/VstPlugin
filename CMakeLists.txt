###########################################
# CMake Project Template for VST Plug-ins #
###########################################

project(vst_plugin)
cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)


if(MSVC)
    add_definitions(/D _CRT_SECURE_NO_WARNINGS)
endif()

############################
# API fix for Linux builds #
############################

if(UNIX AND NOT APPLE)
  add_definitions( -D__cdecl= )
endif()

#################
# Build plug-in #
#################

set(VSTSDK_PATH "/Users/alessandro_fazio/OneDrive - Universita' degli Studi di Roma Tor Vergata/MIS/APD/XCode Projects/VstPlugin/vstsdk2.4" CACHE PATH "Path to VST SDK")
set(VSTSDK_INCLUDE_DIR ${VSTSDK_PATH})
set(VSTSDK_PLUGIN_SOURCE
  ${VSTSDK_PATH}/public.sdk/source/vst2.x/audioeffectx.cpp
  ${VSTSDK_PATH}/public.sdk/source/vst2.x/audioeffect.cpp
  ${VSTSDK_PATH}/public.sdk/source/vst2.x/vstplugmain.cpp
  ${VSTSDK_PATH}/pluginterfaces/vst2.x/aeffectx.h
)

set(VSTPLUGIN_SOURCE
    src/VstPlugin.cpp
    src/VstPlugin.hpp
    src/BufferFactory.hpp
    src/Delay.cpp
    src/Delay.hpp
    src/Oscillator.cpp
    src/Oscillator.hpp
    src/GainStereo.cpp
    src/GainStereo.hpp
    src/ModOperator.cpp
    src/ModOperator.hpp
    src/VDelay.cpp
    src/VDelay.hpp
    src/Chorus.cpp
    src/Chorus.hpp
    src/Interp.hpp
    src/Smooth.hpp
    src/AutoPan.hpp
    #resources/again.rc
    #resources/vstplug.def
    ${VSTSDK_PLUGIN_SOURCE}
)

add_library(vst_plugin MODULE ${VSTPLUGIN_SOURCE})

include_directories(${VSTSDK_INCLUDE_DIR})
target_link_libraries(vst_plugin ${VSTSDK_LIBRARIES})

######################################
# Set OSX-specific bundle properties #
######################################


set_target_properties(vst_plugin PROPERTIES
  BUNDLE true
  BUNDLE_EXTENSION "vst"
  XCODE_ATTRIBUTE_WRAPPER_EXTENSION "vst"
  MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist.in"
  MACOSX_BUNDLE_BUNDLE_NAME "VstPlugin"
  MACOSX_BUNDLE_GUI_IDENTIFIER "com.mis-apd.VstPlugin"
  MACOSX_BUNDLE_ICON_FILE ""
  MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
  MACOSX_BUNDLE_COPYRIGHT "mis-apd © 2020"
)

####################
# Set Install Path #
####################

if(APPLE)
  install(TARGETS vst_plugin
    DESTINATION "/Library/Audio/Plug-Ins/VST"
  )
elseif(WIN32)
  install(TARGETS vst_plugin
    DESTINATION "/Program Files (x86)/Common Files/VST3/"
  )
elseif(UNIX AND NOT APPLE) #Linux
  install(TARGETS vst_plugin
    DESTINATION "/usr/lib/lxvst"
  )
endif()
