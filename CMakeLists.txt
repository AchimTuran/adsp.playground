include(cmake/addon-template-helpers.cmake)

project(adsp.playground C CXX)

cmake_minimum_required(VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(Kodi REQUIRED)
find_package(kodiplatform REQUIRED)
find_package(p8-platform REQUIRED)
find_package(TinyXML REQUIRED)

include(cmake/helpers/gitCloneProject.cmake)
# Include asplib
if(NOT ASPLIB_MODULES_TO_BUILD)
  set(ASPLIB_MODULES_TO_BUILD Dynamics Biquad)
endif()
git_clone_project(PROJ                asplib
                  GIT_REPOSITORY      https://github.com/AchimTuran/asplib
                  GIT_TAG             adding_dynamics_module)
add_subdirectory(${asplib_SOURCE_DIR} ${asplib_BINARY_DIR})

add_subdirectory(src/KodiThreads)
set_property(TARGET KodiThreads PROPERTY FOLDER "depends")
#set_target_properties(kodi::KodiThreads PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

add_subdirectory(src/adsp.template)
set_property(TARGET adsp.template PROPERTY FOLDER "depends")
#set_target_properties(kodi::KodiThreads PROPERTIES OUTPUT_NAME asplib_${PROJECT_NAME})

include_directories(${kodiplatform_INCLUDE_DIRS}
                    ${p8-platform_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/src)
                    #${PROJECT_SOURCE_DIR}/src/template)
                    #${PROJECT_SOURCE_DIR}/src/KodiThreads)
                                          
set(ADSP_PLAYGROUND_SOURCES   #src/ADDONOptional.cpp
                              #src/channelUpmixer.cpp
                              #src/DSPProcessor.cpp
                              src/Dialogs/GUIDialogMasterProcess.cpp
                              src/Dialogs/GUIDialogOutputResample.cpp
                              src/Dialogs/GUIDialogPostProcess.cpp
                              src/Dialogs/GUIDialogPreProcess.cpp
                              
                              #src/ADDONOptional.h
                              #src/channelUpmixer.h
                              #src/DSPProcessor.h
                              src/Dialogs/GUIDialogMasterProcess.h
                              src/Dialogs/GUIDialogOutputResample.h
                              src/Dialogs/GUIDialogPostProcess.h
                              src/Dialogs/GUIDialogPreProcess.h

                              # Processing Modes
                              src/PreProcessCpyModeSettings.cpp
                              src/PreProcessCpyModeSettings.hpp
                              src/PreProcessCpy.cpp
                              src/PreProcessCpy.hpp
                              src/PostProcessCpyModeSettings.cpp
                              src/PostProcessCpyModeSettings.hpp
                              src/PostProcessCpy.cpp
                              src/PostProcessCpy.hpp
                              src/InputResampleCpyModeSettings.cpp
                              src/InputResampleCpyModeSettings.hpp
                              src/InputResampleCpy.cpp
                              src/InputResampleCpy.hpp
                              
                              src/GainMode/GainModeMessages.cpp
                              src/GainMode/GainModeMessages.hpp
                              src/GainMode/GainModeModel.cpp
                              src/GainMode/GainModeModel.hpp
                              src/GainMode/GainModeModelMessages.cpp
                              src/GainMode/GainModeModelMessages.hpp
                              src/GainMode/GainModeSettings.cpp
                              src/GainMode/GainModeSettings.hpp
                              src/GainMode/GainMode.cpp
                              src/GainMode/GainMode.hpp
                              src/GainMode/GainModeDialogSettings.cpp
                              src/GainMode/GainModeDialogSettings.hpp
                              src/GainMode/GainModeDialog.cpp
                              src/GainMode/GainModeDialog.hpp
                              src/GainMode/GainModeDialogMessages.cpp
                              src/GainMode/GainModeDialogMessages.hpp
                              src/GainMode/GainModeController.cpp
                              src/GainMode/GainModeController.hpp
                              src/GainMode/GainModeControllerMessages.cpp
                              src/GainMode/GainModeControllerMessages.hpp
                              src/GainModeEnvironment.cpp
                              src/GainModeEnvironment.hpp
                              
                              src/CompressorMode/CompressorMode.cpp
                              src/CompressorMode/CompressorMode.hpp
                              src/CompressorMode/CompressorModeController.cpp
                              src/CompressorMode/CompressorModeController.hpp
                              src/CompressorMode/CompressorModeControllerMessages.cpp
                              src/CompressorMode/CompressorModeControllerMessages.hpp
                              src/CompressorMode/CompressorModeDialog.cpp
                              src/CompressorMode/CompressorModeDialog.hpp
                              src/CompressorMode/CompressorModeDialogMessages.cpp
                              src/CompressorMode/CompressorModeDialogMessages.hpp
                              src/CompressorMode/CompressorModeDialogSettings.cpp
                              src/CompressorMode/CompressorModeDialogSettings.hpp
                              src/CompressorMode/CompressorModeMessages.cpp
                              src/CompressorMode/CompressorModeMessages.hpp
                              src/CompressorMode/CompressorModeModel.cpp
                              src/CompressorMode/CompressorModeModel.hpp
                              src/CompressorMode/CompressorModeModelMessages.cpp
                              src/CompressorMode/CompressorModeModelMessages.hpp
                              src/CompressorMode/CompressorModeSettings.cpp
                              src/CompressorMode/CompressorModeSettings.hpp
                              src/CompressorModeEnvironment.cpp
                              src/CompressorModeEnvironment.hpp

                              src/EnumStrIDs.cpp
                              src/EnumStrIDs.hpp)
                              
                              #src/CpyModesProcess.cpp
                              #src/CpyModesProcess.hpp)
                       

set(DEPLIBS ${kodiplatform_LIBRARIES}
            ${p8-platform_LIBRARIES}
            kodi::KodiThreads
            adsp::adsp.template
            asplib::Core
            asplib::Dynamics
            asplib::Biquad)
            #winmm.lib)#todo implement switches for Windows

# if(MSVC)
  # list(APPEND ADSP_TEMPLATE_SOURCES src/KodiThreads/threads/platform/win/Win32Exception.cpp)
  # list(APPEND ADSP_TEMPLATE_SOURCES src/KodiThreads/threads/platform/win/Win32Exception.h)
  
  # list(APPEND DEPLIBS winmm.lib)
# endif()

addon_source_group("${ADSP_PLAYGROUND_SOURCES}")
            
build_addon(adsp.playground ADSP_PLAYGROUND DEPLIBS)

# For generating the doxy
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/doxygen_resources/Doxyfile
                    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                    COMMENT "Generating API documentation" VERBATIM)
endif()

include(CPack)

