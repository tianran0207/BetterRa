
#
# chat_server
#

option( BUILD_TESTS "build chat_server executable" ON )

if( BUILD_TESTS )

	set( CHAT_SERVER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}  CACHE INTERNAL "" )
	message( STATUS "Creating target chat_server" )
	file(GLOB CHAT_SERVER_HEADERS ${CHAT_SERVER_SOURCE_DIR}/*.hpp)
	file(GLOB CHAT_SERVER_SOURCES ${CHAT_SERVER_SOURCE_DIR}/*.cpp)
	
	
	set( DEPENDENCIES common)
	set( LIBRARIES ${GLOBAL_LIBRARIES})
	set( INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${COMMON_BASE_INCLUDE_DIRS} )
	set( DEFINITIONS "${GLOBAL_DEFINITIONS} ${COMMON_BASE_DEFINITIONS} -DCMAKE" )
	set( SOURCE_FILES ${COMMON_BASE_HEADERS} ${COMMON_HEADERS} ${CHAT_SERVER_HEADERS} ${CHAT_SERVER_SOURCES} )
	source_group( common FILES ${COMMON_BASE_HEADERS} ${COMMON_HEADERS} )
	source_group( chat FILES ${CHAT_SERVER_HEADERS} ${CHAT_SERVER_SOURCES} )
	include_directories( ${INCLUDE_DIRS} )

	#message( STATUS "chat_server SOURCE_FILES=${SOURCE_FILES}")
	add_executable( chat_server ${SOURCE_FILES} )
	#message( STATUS "chat_server LIBRARIES=${LIBRARIES}, DEPENDENCIES=${DEPENDENCIES} DEFINITIONS=${DEFINITIONS}")
	add_dependencies( chat_server ${DEPENDENCIES} )
	target_link_libraries( chat_server ${LIBRARIES} ${DEPENDENCIES} )
	set_target_properties( chat_server PROPERTIES COMPILE_FLAGS "${DEFINITIONS}" )

	if( INSTALL_COMPONENT_RUNTIME )
		cpack_add_component( Runtime_tests DESCRIPTION "basic integration chat" DISPLAY_NAME "chat_server" GROUP Runtime )
		install( TARGETS chat_server
			DESTINATION "."
			COMPONENT Runtime_tests )
	endif( INSTALL_COMPONENT_RUNTIME )
	set( TARGET_LIST ${TARGET_LIST} chat_server  CACHE INTERNAL "" )
	message( STATUS "Creating target chat_server - done" )

endif( BUILD_TESTS )
