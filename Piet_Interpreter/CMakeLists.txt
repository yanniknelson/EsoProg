# CMakeList.txt : CMake project for EsoProg, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17) # no need to manually adjust the CXXFLAGS

# Add source to this project's executable.
add_executable (EsoProg 
	"Main.cpp" 
	"Main.h" 
	"File_Dialog_Box/FileDialogBox.h" 
	"File_Dialog_Box/FileDialogBox.cpp" 
	"EsoProg.cpp"
	"EsoProg.h"
	"Resources/IconsFontAwesome6.h"
	"Resources/stb_image.h"
	"Resources/stb_image_write.h"
	"PietInterpreter/PietToken.h"
	"PietInterpreter/PietTextTokeniser.h"
	"PietInterpreter/PietTextTokeniser.cpp"
	"PietInterpreter/PietStack.h" 
	"PietInterpreter/PietStack.cpp"
	"PietInterpreter/PietRuntime.h" 
	"PietInterpreter/PietRuntime.cpp"
	"PietInterpreter/PietImageTokeniser.cpp"
	"PietInterpreter/PietImageTokeniser.h"
	"PietInterpreter/PietTokeniser.h"
	"SmartEnum.h"
	"ImGuiHelpers.h" "ImGuiHelpers.cpp")

add_custom_command(TARGET EsoProg POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/Piet_Interpreter/Resources/fa-solid-900.ttf"      # <--this is in-file
        $<TARGET_FILE_DIR:EsoProg>)                 # <--this is out-file path

# TODO: Add tests and install targets if needed.
find_package(glad CONFIG REQUIRED)
target_link_libraries(EsoProg PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(EsoProg PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(EsoProg PRIVATE imgui::imgui)