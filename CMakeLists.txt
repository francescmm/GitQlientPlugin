cmake_minimum_required(VERSION 3.16)

project(GitQlientPlugin)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

execute_process(
   COMMAND
   git -C ${PROJECT_SOURCE_DIR} rev-parse --short HEAD
   RESULT_VARIABLE
   GQ_SHA_RESULT
   OUTPUT_VARIABLE
   GQ_SHA
   OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (NOT DEFINED VERSION OR "${VERSION}" STREQUAL "")
   if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
      set(VERSION "0.0.0")
   else()
      set(VERSION ${GQ_SHA})
   endif()
endif()

add_compile_definitions(SHA_VER="${GQ_SHA}")
add_compile_definitions(VER="${VERSION}")
add_compile_definitions(QT_DEPRECATED_WARNINGS)

if (NOT EXISTS "${PROJECT_SOURCE_DIR}/src/AuxiliarCustomWidgets/.git" OR
    NOT EXISTS "${PROJECT_SOURCE_DIR}/src/QLogger/.git" OR
    NOT EXISTS "${PROJECT_SOURCE_DIR}/src/QPinnableTabWidget/.git" OR
    NOT EXISTS "${PROJECT_SOURCE_DIR}/src/git/.git")
    message(STATUS "Submodule update:")
    execute_process(COMMAND git submodule update --init --recursive
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()

find_package(QtCreator REQUIRED COMPONENTS Core TextEditor)
find_package(Qt6 COMPONENTS Core Widgets Network REQUIRED)

# Add a CMake option that enables building your plugin with tests.
# You don't want your released plugin binaries to contain tests,
# so make that default to 'NO'.
# Enable tests by passing -DWITH_TESTS=ON to CMake.
option(WITH_TESTS "Builds with tests" NO)

if(WITH_TESTS)
  # Look for QtTest
  find_package(Qt6 REQUIRED COMPONENTS Test)

  # Tell CMake functions like add_qtc_plugin about the QtTest component.
  set(IMPLICIT_DEPENDS Qt::Test)

  # Enable ctest for auto tests.
  enable_testing()
endif()

add_subdirectory(GitQlient/src/aux_widgets)
add_subdirectory(GitQlient/src/AuxiliarCustomWidgets)
add_subdirectory(GitQlient/src/big_widgets)
add_subdirectory(GitQlient/src/branches)
add_subdirectory(GitQlient/src/cache)
add_subdirectory(GitQlient/src/commits)
add_subdirectory(GitQlient/src/config)
add_subdirectory(GitQlient/src/diff)
add_subdirectory(GitQlient/src/git)
add_subdirectory(GitQlient/src/history)
add_subdirectory(GitQlient/src/QLogger)
add_subdirectory(GitQlient/src/QPinnableTabWidget)

qt_add_resources(
  RESOURCES
    GitQlientPlugin.qrc
    GitQlient/src/GitQlient.qrc
    GitQlient/src/AuxiliarCustomWidgets/resources.qrc
)

add_qtc_plugin(GitQlientPlugin
  PLUGIN_DEPENDS
    QtCreator::Core
    QtCreator::TextEditor
    QtCreator::ProjectExplorer
  DEPENDS
    Qt::Widgets
    Qt::Core
    Qt::Network
    QtCreator::ExtensionSystem
    QtCreator::Utils
    big_widget
  PLUGIN_DEPENDS QtCreator::Core
  SOURCES
    .github/workflows/build_cmake.yml
    .github/workflows/README.md
    README.md
    GitQlientPlugin.cpp
    gitqlientpluginconstants.h
    gitqlientplugintr.h
    GitQlientPlugin.qrc
    GitQlient/src/GitQlient.qrc
    ${RESOURCES}
)

target_sources(
  GitQlientPlugin
  PRIVATE
    ${RESOURCES}
)

# Enable the Run button in Qt Creator
get_target_property(QtCreatorCorePath QtCreator::Core LOCATION)
find_program(QtCreatorExecutable
  NAMES
    qtcreator "Qt Creator"
  PATHS
    "${QtCreatorCorePath}/../../../bin"
    "${QtCreatorCorePath}/../../../MacOS"
  NO_DEFAULT_PATH
)
if (QtCreatorExecutable)
  add_custom_target(RunQtCreator
    COMMAND ${QtCreatorExecutable} -pluginpath $<TARGET_FILE_DIR:GitQlientPlugin>
    DEPENDS GitQlientPlugin
  )
  set_target_properties(RunQtCreator PROPERTIES FOLDER "qtc_runnable")
endif()
