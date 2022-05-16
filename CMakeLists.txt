cmake_minimum_required(VERSION 3.5.0)

project(classifier)
set(CMAKE_CXX_STANDARD 14)

# Sources
file(GLOB SOURCE_FILES "src/classifier/*.cpp" 
					   "src/cvtypes/*.cpp"
					   "src/extras/*.cpp"
					   "src/io/*.cpp"
					   "src/jserrors/*.cpp"
					   "src/utils/*.cpp"
					   "src/*.cpp"
					   )

				
find_package( OpenCV 4 COMPONENTS core highgui imgproc imgcodecs REQUIRED)
find_package( Torch REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")


add_library(${PROJECT_NAME} ${SRCS} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC} "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api")
target_link_libraries(${PROJECT_NAME} ${TORCH_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} )

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

if(WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/wd4624")
endif(WIN32)