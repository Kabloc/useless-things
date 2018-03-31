file( GLOB SOURCES "*.cpp" )
add_definitions( -g -std=c++11 )
add_executable( evaluation ${SOURCES} )
