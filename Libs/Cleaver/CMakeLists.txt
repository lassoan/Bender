#============================================================================
#
# Program: Bender
#
# Copyright (c) Kitware Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0.txt
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#============================================================================

cmake_minimum_required(VERSION 2.6)

set(KIT Cleaver)
project(Bender${KIT})
string(TOUPPER ${KIT} KIT_UPPER)

#-----------------------------------------------------------------------------
# Get ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#-----------------------------------------------------------------------------
# Get VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#-----------------------------------------------------------------------------
# Get Cleaver
find_package(Cleaver REQUIRED)
include(${Cleaver_USE_FILE})

#-----------------------------------------------------------------------------
# Create library

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Cleaver_INCLUDE_DIRS}
  )

set(${KIT}_SRCS
  )

#add_library(${PROJECT_NAME} ${${KIT}_SRCS})
#target_link_libraries(${PROJECT_NAME}
#  ${ITK_LIBRARIES}
#  ${VTK_LIBRARIES}
#  ${Cleaver_LIBRARIES}
#  )
#bender_export_library()

#install(TARGETS ${PROJECT_NAME}
#  RUNTIME DESTINATION ${Bender_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
#  LIBRARY DESTINATION ${Bender_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
#  ARCHIVE DESTINATION ${Bender_INSTALL_LIB_DIR} COMPONENT Development
#  )

#-----------------------------------------------------------------------------
# Configure export file

#set(MyLibraryExportDirective "BENDER_${KIT_UPPER}_EXPORT")
#set(MyExportHeaderPrefix ${PROJECT_NAME})
#set(MyLibName ${PROJECT_NAME})

#configure_file(
#  ${Bender_SOURCE_DIR}/CMake/BenderExport.h.in
#  ${PROJECT_NAME}Export.h
#  )

#set(dynamicHeaders
#  "${dynamicHeaders};${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Export.h")

set_property(GLOBAL APPEND PROPERTY Bender_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

#-----------------------------------------------------------------------------
# Add testing
if (BUILD_TESTING)
  add_subdirectory(Testing)
endif()
