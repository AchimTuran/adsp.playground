project(adsp.template)

#find_package(p8-platform REQUIRED)

set(SOURCES ADSPHelpers.cpp
            client.cpp
            GUIDialogBase.cpp
            #Addon/UI/Controller.cpp
            #Addon/UI/Model.cpp
            #Addon/UI/Observer.cpp
            #Addon/UI/View.cpp
            Addon/MessageSystem/Communication/Message.cpp
            Addon/MessageSystem/Communication/ActorProtocol.cpp
            Addon/MessageSystem/Communication/MessageDispatcher.cpp
            Addon/MessageSystem/Sockets/TSocketMemcpy.hpp
            Addon/Process/AddonProcessManager.cpp
            AudioDSP/FactoryADSPModes/FactoryADSPModes.cpp
            AudioDSP/Streams/ADSPStreamManager.cpp
            AudioDSP/Streams/ADSPStream.cpp
            AudioDSP/Streams/ADSPStreamBuilderAll.cpp
            Settings/SettingsHelpers.cpp
            Settings/SettingsManager.cpp)

set(HEADERS ADSPHelpers.h
            doxygenDocu.h
            Addon/Utils/EnumIDsToString.hpp
            Addon/MVC/Interfaces/MVCObject.hpp
            Addon/MVC/Interfaces/Controller/IController.hpp
            Addon/MVC/Interfaces/Model/IModel.hpp
            Addon/MVC/Interfaces/Model/IParameter.hpp
            Addon/MVC/Model/TParameter.hpp
            Addon/MessageSystem/Interfaces/IProtocol.hpp
            Addon/MessageSystem/Communication/ActorProtocol.h
            Addon/MessageSystem/Communication/MessageDispatcher.hpp
            Addon/MessageSystem/Communication/Message.hpp
            Addon/MessageSystem/Sockets/TSocketClassMethodCallback.hpp
            Addon/MessageSystem/Sockets/TSocketRangeCheck.hpp
            Addon/MessageSystem/Sockets/TSocketNotify.hpp
            #Addon/MessageSystem/Communication/ActorPort.hpp
            #Addon/MessageSystem/Interfaces/IPort.hpp
            #Addon/MessageSystem/Interfaces/IPortPool.hpp
            Addon/MessageSystem/Interfaces/ISocket.hpp
            #Addon/MessageSystem/Slots/TSlotMemcpy.hpp
            Addon/Process/AddonProcessManager.hpp
            Addon/Process/IAddonProcess.hpp
            AddonExceptions/IAddonException.h
            AddonExceptions/TAddonException.h
            configuration/templateConfiguration.h
            include/checkTemplateConfig.h
            include/client.h
            include/GUIDialogBase.h
            include/MACROHelper.h
            include/typedefs.h
            Settings/ISettingsElement.h
            Settings/SettingsHelpers.h
            Settings/SettingsManager.h
            Settings/TSettingsElement.h
            
            #Addon/Settings/SettingsPool.hpp
            #Addon/UI/BaseDialog.hpp
            #Addon/UI/Controller.hpp
            #Addon/UI/Model.hpp
            #Addon/UI/Observer.hpp
            Addon/MVC/Interfaces/View/IView.hpp
            Addon/MVC/Interfaces/View/IKodiGUIView.hpp
            Addon/MVC/Interfaces/View/ActionIDs.hpp
            Addon/Utils/TCreationStatistics.hpp
            AudioDSP/FactoryADSPModes/FactoryADSPModes.hpp
            AudioDSP/FactoryADSPModes/IADSPMode.hpp
            AudioDSP/Streams/ADSPStreamManager.hpp
            AudioDSP/Streams/ADSPStream.hpp
            AudioDSP/Streams/IADSPStreamBuilder.hpp
            AudioDSP/Streams/ADSPStreamBuilderAll.hpp
            
            utils/stdStringUtils.h)
          
addon_source_group("${SOURCES}")
addon_source_group("${HEADERS}")

include_directories(${kodiplatform_INCLUDE_DIRS}
                    ${p8-platform_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(adsp::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
                           
target_link_libraries(${PROJECT_NAME} kodi::KodiThreads)

# install all specific module files
#asplib_install_module(${PROJECT_NAME} "${HEADERS}" ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ${PROJECT_NAME} EXPORT adsp DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
