project "EsoProg"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"

   files { "Src/**.h", "Src/**.cpp", "README.md"}

   includedirs
   {
      "Src",

	  -- Include Core
	  "%{wks.location}/Core/SmartEnums/Src",
	  "%{wks.location}/Core/Logger/Src",
	  "%{wks.location}/Core/LanguageHelpers/ELanguages/Src",
	  "%{wks.location}/Core/ImGuiHelpers/FileDialogBox",
	  "%{wks.location}/Core/ImGuiHelpers/ImGuiSetStyles",
	  "%{wks.location}/Core/ImGuiHelpers/ImGuiValueChangeCallbacks",
      "%{wks.location}/Core/LanguageHelpers/ITokeniser/Src",
      "%{wks.location}/Core/LanguageHelpers/IAST/Src",
      "%{wks.location}/Core/LanguageHelpers/IParser/Src",
      "%{wks.location}/Core/LanguageHelpers/IStack/Src",
	  "%{wks.location}/Core/LanguageHelpers/IMemoryArray/Src",
	  "%{wks.location}/Core/LanguageHelpers/IRuntime/Src",
	  "%{wks.location}/Core/Interpreters/NullInterpreter/Src",
	  "%{wks.location}/Core/Interpreters/PietInterpreter/Src",
	  "%{wks.location}/Core/Interpreters/BrainFckInterpreter/Src",

      -- Include externals
      "%{wks.location}/%{externals.spdlog}/include",
      "%{wks.location}/%{externals.stb}",
      "%{wks.location}/%{externals.imgui}",
      "%{wks.location}/%{externals.imgui}/misc/cpp",
      "%{wks.location}/%{externals.imgui}/backends",
      "%{wks.location}/%{externals.imgui}/examples/libs/glfw/include",
      "%{wks.location}/%{externals.FontAwesomeHeader}"
   }

   libdirs
   {
       "%{wks.location}/%{externals.imgui}/examples/libs/glfw/lib-vc2010-64"
   }

   links
   {
      "spdlog",
      "glfw3",
      "gdi32",
      "opengl32",
      "imm32",
      "imgui",
      "Logger",
      "ImGuiFileDialogBox",
      "ImGuiSetStyles",
      "ImGuiValueChangeCallbacks",
      "IStack",
      "PietInterpreter",
      "BrainFckInterpreter"
   }

   targetdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir)
   objdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir .. "/Intermediates")

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"
       kind "WindowedApp"