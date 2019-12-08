CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT (female_male_predicter)

set(CMAKE_INCLUDE_CURRENT_DIR on)
set(CMAKE_PREFIX_PATH ~/Library/Qt/5.10.0/clang_64)
set(CMAKE_AUTOMOC ON)



# Bibliotheken finden
SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE( Boost 1.57 COMPONENTS system date_time filesystem program_options serialization thread unit_test_framework chrono atomic REQUIRED )




FIND_PACKAGE( Qt5Widgets REQUIRED )
FIND_PACKAGE( OpenCV REQUIRED )

FIND_PACKAGE( Shark REQUIRED )
ADD_DEFINITIONS(-DNOMINMAX) # Sonst bekommt Shark Probleme
FIND_PACKAGE(Qt5Core REQUIRED)

INCLUDE_DIRECTORIES(    
	${Boost_INCLUDE_DIRS} 
	${CMAKE_CURRENT_BINARY_DIR}
	../include
	${OpenCV_INCLUDE_DIRS}
	${SHARK_INCLUDE_DIRS}
)

SET( INCS
../include/MainWindow.h
../include/Brain.h
../include/TrainWidget.h
../include/LoadDataWidget.h
)


SET( SRCS
../src/MainWindow.cpp
../src/Brain.cpp
../src/TrainWidget.cpp
../src/LoadDataWidget.cpp
../src/main.cpp
)

SET( GUIS
../gui/MainWindow.ui
../gui/LoadDataWidget.ui
../gui/TrainWidget.ui
)

QT5_WRAP_UI( GUI_SRCS
${GUIS}
)

ADD_EXECUTABLE( ${PROJECT_NAME}
	${INCS}
	${SRCS}
	${GUI_SRCS}
)

TARGET_LINK_LIBRARIES( ${PROJECT_NAME} 
	${OpenCV_LIBS}
	${Boost_LIBRARIES}
	${SHARK_LIBRARIES}
	Qt5::Widgets
    Qt5::Core
)
