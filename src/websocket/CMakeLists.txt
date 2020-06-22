
#
# websocket_server
#

option( BUILD_TESTS "build websocket_server executable" ON )

if( BUILD_TESTS )

	set( WEBSOCKET_SERVER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}  CACHE INTERNAL "" )
	message( STATUS "Creating target websocket_server" )
	file(GLOB WEBSOCKET_SERVER_HEADERS ${WEBSOCKET_SERVER_SOURCE_DIR}/*.hpp)
	file(GLOB WEBSOCKET_SERVER_SOURCES ${WEBSOCKET_SERVER_SOURCE_DIR}/*.cpp)
	
	
	set( DEPENDENCIES common)
	set( LIBRARIES ${GLOBAL_LIBRARIES})
	set( INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${COMMON_BASE_INCLUDE_DIRS} )
	set( DEFINITIONS "${GLOBAL_DEFINITIONS} ${COMMON_BASE_DEFINITIONS} -DCMAKE" )
	set( SOURCE_FILES ${COMMON_BASE_HEADERS} ${COMMON_HEADERS} ${WEBSOCKET_SERVER_HEADERS} ${WEBSOCKET_SERVER_SOURCES} )
	source_group( common FILES ${COMMON_BASE_HEADERS} ${COMMON_HEADERS} )
	source_group( websocket FILES ${WEBSOCKET_SERVER_HEADERS} ${WEBSOCKET_SERVER_SOURCES} )
	include_directories( ${INCLUDE_DIRS} )

	#message( STATUS "websocket_server SOURCE_FILES=${SOURCE_FILES}")
	add_executable( websocket_server ${SOURCE_FILES} )
	#message( STATUS "websocket_server LIBRARIES=${LIBRARIES}, DEPENDENCIES=${DEPENDENCIES} DEFINITIONS=${DEFINITIONS}")
	add_dependencies( websocket_server ${DEPENDENCIES} )
	target_link_libraries( websocket_server ${LIBRARIES} ${DEPENDENCIES} )
	set_target_properties( websocket_server PROPERTIES COMPILE_FLAGS "${DEFINITIONS}" )

	if( INSTALL_COMPONENT_RUNTIME )
		cpack_add_component( Runtime_tests DESCRIPTION "basic integration websocket" DISPLAY_NAME "websocket_server" GROUP Runtime )
		install( TARGETS websocket_server
			DESTINATION "."
			COMPONENT Runtime_tests )
	endif( INSTALL_COMPONENT_RUNTIME )
	set( TARGET_LIST ${TARGET_LIST} websocket_server  CACHE INTERNAL "" )
	message( STATUS "Creating target websocket_server - done" )

endif( BUILD_TESTS )
