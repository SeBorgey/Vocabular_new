cmake_minimum_required(VERSION 3.5)

project(Vocabular VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/currentlearning.cpp
        src/currentlearning.h
        src/learnui.cpp
        src/learnui.h
        src/mainmenu.cpp
        src/mainmenu.h
        src/mainvocabulary.cpp
        src/mainvocabulary.h
)

if(NOT ANDROID)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(AVFORMAT REQUIRED IMPORTED_TARGET libavformat)
    pkg_check_modules(AVCODEC REQUIRED IMPORTED_TARGET libavcodec)
    pkg_check_modules(AVUTIL REQUIRED IMPORTED_TARGET libavutil)
    pkg_check_modules(SWSCALE REQUIRED IMPORTED_TARGET libswscale)
    pkg_check_modules(SWRESAMPLE REQUIRED IMPORTED_TARGET libswresample)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia MultimediaWidgets)

    list(APPEND PROJECT_SOURCES
        src/srtparser.h
        src/backend.h
        src/backend.cpp
        src/multiselectlabel.h
        src/multiselectlabel.cpp
        src/subtitleextractor.h
        src/subtitleextractor.cpp
        src/subtitleselectiondialog.h
        src/subtitleselectiondialog.cpp
        src/mainwindow.h
        src/mainwindow.cpp
        src/videoplayerui.h
        src/videoplayerui.cpp
        src/videoplayercontroller.h
        src/videoplayercontroller.cpp
        src/subtitlemanager.h
        src/subtitlemanager.cpp
        src/hovereventfilter.h
        src/hovereventfilter.cpp
        src/multiselecttextedit.h
        src/multiselecttextedit.cpp
        src/customlabel.h
        src/customlabel.cpp
        src/utilities.h
        src/wordeditor.h
        src/wordeditor.cpp
        src/translator.h src/translator.cpp
    )
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Vocabular
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}





    )
else()
    if(ANDROID)
        add_library(Vocabular SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Vocabular
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Vocabular PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(NOT ANDROID)
    target_link_libraries(Vocabular PRIVATE
        Qt${QT_VERSION_MAJOR}::Multimedia
        Qt${QT_VERSION_MAJOR}::MultimediaWidgets
        PkgConfig::AVFORMAT
        PkgConfig::AVCODEC
        PkgConfig::AVUTIL
        PkgConfig::SWSCALE
        PkgConfig::SWRESAMPLE
    )
endif()

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Vocabular)
endif()

set_target_properties(Vocabular PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(ANDROID)
    set_target_properties(Vocabular PROPERTIES
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
    )
endif()

include(GNUInstallDirs)
install(TARGETS Vocabular
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Vocabular)
endif()
