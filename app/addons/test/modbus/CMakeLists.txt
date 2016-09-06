IF( APPLE )
  INCLUDE_DIRECTORIES(
	../../prebuild-mac/include
  )
ELSE()
  MESSAGE( FATAL_ERROR "Unsupport platform" )
ENDIF()

ADD_EXECUTABLE( modbus-test
	main.c
)
#TARGET_LINK_LIBRARIES( 
#)
