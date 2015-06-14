# Hierarchical
cmake_minimum_required(VERSION 2.8)

# project name
project(Hierarchical)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR} )

ADD_DEFINITIONS( -O2 -ggdb )

# find dependencies 
find_package(OpenCV REQUIRED)


add_library(Hierarchical
	STATIC
	Node.cpp Hierarchy.cpp HierarchicalWrapper.cpp
)
target_link_libraries(Hierarchical
	${OpenCV_LIBS}
)

add_executable(NodeTest
	NodeTest.cpp
)
target_link_libraries(NodeTest
	Hierarchical
)


###############################################################################
## 								Install
# Binaries and libs
#install(TARGETS LBP LBPMain
#	RUNTIME DESTINATION bin
#	LIBRARY DESTINATION lib
#	ARCHIVE DESTINATION lib
#)
# Header files
#install(FILES LBP.hpp
#	DESTINATION include 
#)
