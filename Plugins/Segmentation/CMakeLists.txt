PROJECT(Segmentation)
IF(${CMAKE_VERSION} VERSION_GREATER 3.1)
    CMAKE_POLICY(SET CMP0053 OLD)
ENDIF()
CMAKE_POLICY(SET CMP0020 NEW)

IF(NOT WIN32)
    set(BOOST_REQUIRED_MODULES system filesystem thread date_time iostreams chrono)
ENDIF(NOT WIN32)

find_package(Boost 1.47.0 QUIET COMPONENTS ${BOOST_REQUIRED_MODULES})

find_package(CUDA REQUIRED)

find_package(OpenCV 3.0 QUIET COMPONENTS core imgproc highgui cudaimgproc cudawarping cudafeatures2d cudaoptflow cudacodec cudastereo)
set_target_properties(${OpenCV_LIBS} PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)

ADD_SUBDIRECTORY(fastms)
set_target_properties(fastms PROPERTIES FOLDER Dependencies)
INCLUDE_DIRECTORIES(
        ${INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${CUDA_INCLUDE_DIRS}
)

LINK_DIRECTORIES(${LINK_DIRS})
file(GLOB src "*.cpp")
file(GLOB hdr "*.h" "*.hpp" "*.cuh")
file(GLOB knl "*.cu")

cuda_add_library(Segmentation SHARED ${src} ${hdr})

add_dependencies(Segmentation Aquila)

RCC_LINK_LIB(Segmentation
    Aquila
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
)


IF(TARGET fastms)

    add_dependencies(Segmentation fastms)
    RCC_LINK_LIB(Segmentation fastms ${fastms_LIBS})
    TARGET_LINK_LIBRARIES(Segmentation fastms)
    add_definitions(-DFASTMS_FOUND)
    add_definitions(-DFASTMS_INCLUDE=${fastms_INCLUDE_DIRECTORIES})
    LIST(APPEND DEFS -DFASTMS_FOUND)
    INCLUDE_DIRECTORIES(${fastms_INCLUDE_DIRECTORIES})
    #message(STATUS "FastMS include: ${fastms_INCLUDE_DIRECTORIES}")

ENDIF()
INCLUDE(../PluginTemplate.cmake)
