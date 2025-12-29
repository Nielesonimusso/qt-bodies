cmake_minimum_required(VERSION 3.16)

project(qt_bodies VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# n-bodies library creation from the original sources
include(FetchContent)
cmake_policy(SET CMP0169 OLD)

FetchContent_Declare(
    nbodies_loethor
    GIT_REPOSITORY https://github.com/Loethor/n-bodies.git
    GIT_TAG main
)

FetchContent_Populate(nbodies_loethor)

set(nbodies_SOURCES
    "${nbodies_loethor_SOURCE_DIR}/src/body/body.cpp"
    "${nbodies_loethor_SOURCE_DIR}/src/integrator/euler_integrator.cpp"
    "${nbodies_loethor_SOURCE_DIR}/src/simulation/simulation.cpp"
)

file(GLOB nbodies_DATA "${nbodies_loethor_SOURCE_DIR}/data/*.txt")
foreach (f ${nbodies_DATA})
    get_filename_component(fname "${f}" NAME)

    set_source_files_properties("${f}"
        PROPERTIES QT_RESOURCE_ALIAS "data/${fname}"
    )
endforeach()
message(NOTICE "nbodies data: ${nbodies_DATA}")

add_library(nbodies STATIC "${nbodies_SOURCES}")
target_include_directories(nbodies PUBLIC "${nbodies_loethor_SOURCE_DIR}/include")

# end n-bodies library

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appqt_bodies
    main.cpp
)

qt_add_qml_module(appqt_bodies
    URI qt_bodies
    QML_FILES
        Main.qml
        SOURCES
            nbodies.h
            nbodies.cpp
            bodyqml.h
        RESOURCES
            ${nbodies_DATA}
            RESOURCES LICENSE
            RESOURCES copyifexists.cmake
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appqt_bodies PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appqt_bodies
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqt_bodies
    PRIVATE Qt6::Quick nbodies
)

include(GNUInstallDirs)
install(TARGETS appqt_bodies
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET appqt_bodies POST_BUILD
    COMMAND ${CMAKE_COMMAND}
        -DSRC="${CMAKE_BINARY_DIR}/appqt_bodies.html"
        -DDST="${CMAKE_SOURCE_DIR}/docs"
        -DNAME="index.html"
        -P "${CMAKE_SOURCE_DIR}/copyifexists.cmake"
    COMMAND ${CMAKE_COMMAND}
        -DSRC="${CMAKE_BINARY_DIR}/appqt_bodies.js"
        -DDST="${CMAKE_SOURCE_DIR}/docs"
        -P "${CMAKE_SOURCE_DIR}/copyifexists.cmake"
    COMMAND ${CMAKE_COMMAND}
        -DSRC="${CMAKE_BINARY_DIR}/appqt_bodies.wasm"
        -DDST="${CMAKE_SOURCE_DIR}/docs"
        -P "${CMAKE_SOURCE_DIR}/copyifexists.cmake"
    COMMAND ${CMAKE_COMMAND}
        -DSRC="${CMAKE_BINARY_DIR}/qtloader.js"
        -DDST="${CMAKE_SOURCE_DIR}/docs"
        -P "${CMAKE_SOURCE_DIR}/copyifexists.cmake"
    COMMAND ${CMAKE_COMMAND}
        -DSRC="${CMAKE_BINARY_DIR}/qtlogo.svg"
        -DDST="${CMAKE_SOURCE_DIR}/docs"
        -P "${CMAKE_SOURCE_DIR}/copyifexists.cmake"
    COMMAND ${CMAKE_COMMAND}
        -DSRC="${CMAKE_BINARY_DIR}/qt_bodies"
        -DDST="${CMAKE_SOURCE_DIR}/docs"
        -P "${CMAKE_SOURCE_DIR}/copyifexists.cmake"
)
