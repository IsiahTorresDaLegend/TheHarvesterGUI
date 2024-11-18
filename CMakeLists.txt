cmake_minimum_required(VERSION 3.14)

project(TheHarvesterGUI)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport)

# Find Python libraries
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Add the executable target
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
    main.cpp
    mainwindow.cpp
    pythonworker.cpp
    fileoperations.cpp
    formatsettings.cpp
    sourcesmanager.cpp
    workercontroller.cpp
    collapsiblegroupbox.cpp
)

set(HEADERS
    mainwindow.h
    mainwindow.ui
    pythonworker.h
    fileoperations.h
    formatsettings.h
    sourcesmanager.h
    workercontroller.h
    collapsiblegroupbox.h
)

set(UI_FILES
    mainwindow.ui
)

set(RESOURCE_FILES
    resources.qrc
)

qt6_add_resources(RESOURCES ${RESOURCE_FILES})

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCES}
)





# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::PrintSupport)

# Include Python headers (if needed)
target_include_directories(${PROJECT_NAME} PRIVATE ${Python3_INCLUDE_DIRS})

# Link Python libraries (if needed)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Python3_LIBRARIES})

# Define the Python interpreter path as a compile definition
target_compile_definitions(${PROJECT_NAME} PRIVATE PYTHON_EXECUTABLE="${Python3_EXECUTABLE}")

# Install Python requirements
add_custom_target(install_requirements ALL
    COMMAND ${Python3_EXECUTABLE} -m pip install -r ${CMAKE_SOURCE_DIR}/theHarvester/requirements.txt
    COMMENT "Installing Python requirements for TheHarvester..."
)
