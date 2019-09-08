# Qt
set(QTDIR /Users/shenyaoke/Qt/5.12.0/clang_64/)
set(CMAKE_PREFIX_PATH ${QTDIR})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(RESOURCE_FILE ../resources/Kaguya.qrc)
set(VIEWER_FILES MainWindow.cpp OGLViewer.cpp NewGLView.cpp ResolutionGate.cpp main.cpp)
set(CMAKE_AUTOUIC_SEARCH_PATHS ../resources/forms)

add_executable(KaguyaViewerQt ${VIEWER_FILES} ${RESOURCE_FILE})
target_link_libraries(KaguyaViewerQt KaguyaCore)

target_link_libraries(KaguyaViewerQt Qt5::Widgets Qt5::Gui Qt5::Core Qt5::OpenGL)


