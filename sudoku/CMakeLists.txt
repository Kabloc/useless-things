file( GLOB SOURCES "*.cpp" )
add_definitions( -g )
add_executable( sudoku ${SOURCES} )
