cmake_minimum_required(VERSION 2.8)
 


set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")



PROJECT(vtktp)



IF(APPLE)

SET( OperatingSystem "Mac os X")
set (MY_MESHES_PATH  /Users/emmanuelmelin/shared_folders/data_VTK)


find_package(VTK 9 REQUIRED)

ENDIF (APPLE)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

SET(OperatingSystem "Linux")

set (MY_MESHES_PATH  /tp)

find_package(VTK)
include(${VTK_USE_FILE})

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


MESSAGE(WARNING "System detected:${OperatingSystem}" )
MESSAGE(WARNING "Path to data:${MY_MESHES_PATH}" )


configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
 

file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp )

include_directories("${PROJECT_BINARY_DIR}")


 
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})



add_executable(${PROJECT_NAME} ${SRC_LIST})

 
target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES} ${MPI_LIBRARIES})





if(MPI_COMPILE_FLAGS)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()


add_custom_target(prun1
    COMMAND mpirun  --oversubscribe  -np 1 ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

add_custom_target(prun2
    COMMAND mpirun  --oversubscribe -np 2 ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

add_custom_target(prun4
    COMMAND mpirun  --oversubscribe  -np 4 ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)




enable_testing()
add_test(SimpleTest ${PROJECT_NAME})
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS ${PROJECT_NAME})
