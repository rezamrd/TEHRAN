#..............................................................
# CMakeList.txt : CMake project for Sofia, include source and define
# project specific logic here.
#..............................................................

cmake_minimum_required (VERSION 3.8)

project ("TEHRAN")

# Add source to this project's executable: check name and dependences
add_executable (TEHRAN
	"Compilers.h"	"Compilers.c"

	"Buffer.h"		"Buffer.c"		"MainBuffer.c"
	
	"Scanner.c"     "Scanner.h"     "MainScanner.c"
)

# TO_DO: Add tests and install targets if needed.
