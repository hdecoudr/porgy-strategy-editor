cmake_minimum_required(VERSION 3.9)

project(porgy-strategy-editor)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(
    ${PROJECT_NAME}
    ui/codeeditorview.ui
    ui/mainwindow.ui
    resources/code-editor-icons.qrc
    resources/code-editor-styles.qrc
    resources/code-editor-syntax.qrc
    include/codeeditorlistview.h
    include/codeeditorplaintextview.h
    include/codeeditorview.h
    include/generalpurposesyntaxhighlighter.h
    include/mainwindow.h
    sources/codeeditorlistview.cpp
    sources/codeeditorplaintextview.cpp
    sources/codeeditorview.cpp
    sources/generalpurposesyntaxhighlighter.cpp
    sources/generalpurposesyntaxhighlightingrules.cpp
    sources/highlightingrule.cpp
    sources/main.cpp
    sources/mainwindow.cpp
    sources/sourcecode.cpp
    sources/sourcecodemodel.cpp
    sources/xmlsyntaxparser.cpp
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)


target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Xml)

