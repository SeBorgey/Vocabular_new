cmake_minimum_required(VERSION 3.5)

project(Vocabular VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVFORMAT REQUIRED IMPORTED_TARGET libavformat)
pkg_check_modules(AVCODEC REQUIRED IMPORTED_TARGET libavcodec)
pkg_check_modules(AVUTIL REQUIRED IMPORTED_TARGET libavutil)
pkg_check_modules(SWSCALE REQUIRED IMPORTED_TARGET libswscale)
pkg_check_modules(SWRESAMPLE REQUIRED IMPORTED_TARGET libswresample)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/srtparser.h
)

# target_link_libraries(Vocabular PRIVATE
#     Qt6::Core
#     Qt6::Multimedia
#     MediaPlayer
# )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Vocabular
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/mainmenu.h src/mainmenu.cpp
        src/learnui.h src/learnui.cpp
        src/backend.h src/backend.cpp






        src/multiselectlabel.h
        src/multiselectlabel.cpp
        src/subtitleextractor.h src/subtitleextractor.cpp
        src/subtitleselectiondialog.h src/subtitleselectiondialog.cpp
        src/mainwindow.h src/mainwindow.cpp
        src/videoplayerui.h src/videoplayerui.cpp
        src/videoplayercontroller.h src/videoplayercontroller.cpp
        src/subtitlemanager.h src/subtitlemanager.cpp
        src/hovereventfilter.h src/hovereventfilter.cpp
        src/multiselecttextedit.h src/multiselecttextedit.cpp
        src/customlabel.h src/customlabel.cpp
        src/utilities.h
        src/mainvocabulary.h src/mainvocabulary.cpp
        src/currentlearning.h src/currentlearning.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Vocabular APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Vocabular SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Vocabular
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Vocabular PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    PkgConfig::AVFORMAT
    PkgConfig::AVCODEC
    PkgConfig::AVUTIL
    PkgConfig::SWSCALE
    PkgConfig::SWRESAMPLE
)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
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

include(GNUInstallDirs)
install(TARGETS Vocabular
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Vocabular)
endif()
