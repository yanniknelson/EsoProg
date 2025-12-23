-- Define option to enable profiler
newoption
{
    trigger = "ENABLE_PROFILER",
    description = "Enable the profiler integration.",
    allowed = {
      { "ON",  "Enable Profiler" },
      { "OFF", "Disable Profiler" }
    },
    default = "OFF"
}

workspace "EsoProg"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "EsoProg"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus", "/utf-8" }
	  defines { "PLATFORM_WINDOWS" }

project "Premake"
	kind "Utility"

	targetdir ("%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}")

	files {
		"%{wks.location}/**.lua",
		"%{wks.location}/LICENSE.md",
		"%{wks.location}/README.md"
	}

	postbuildmessage "Regenerating project files with Premake5!"

        filter "system:windows"
            postbuildcommands {
                "%{wks.location}/Vendor/Binaries/Premake/Windows/premake5.exe %{_ACTION} --file=\"%{wks.location}/Build.lua\""
            }

        filter "system:linux"
            postbuildcommands {
                "%{wks.location}/Vendor/Binaries/Premake/Linux/premake5 %{_ACTION} --file=\"%{wks.location}/Build.lua\""
            }

        filter "system:macosx"
            postbuildcommands {
                "%{wks.location}/Vendor/Binaries/Premake/macOS/premake5 %{_ACTION} --file=\"%{wks.location}/Build.lua\""
            }

        filter {} -- Clear filter

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

externals = {}
externals["spdlog"] = "Vendor/spdlog"
externals["Optick"] = "Vendor/Optick/src"
externals["imgui"] = "Vendor/imgui"
externals["FontAwesomeHeader"] = "Vendor/FontAwesomeHeaders"
externals["stb"] = "Vendor/stb"

group "Vendor"
	include "Vendor/Build-Externals.lua"
group "Core"
group "Core/Common"
group "Core/Common/ImGuiHelpers"
	include "Core/ImGuiHelpers/Build-ImGuiHelpers.lua"
group "Core/Common/LanguageHelpers"
	include "Core/LanguageHelpers/ELanguages/Build-ELanguages.lua"
	include "Core/LanguageHelpers/IAST/Build-IAST.lua"
	include "Core/LanguageHelpers/IMemoryArray/Build-IMemoryArray.lua"
	include "Core/LanguageHelpers/IParser/Build-IParser.lua"
	include "Core/LanguageHelpers/IRuntime/Build-IRuntime.lua"
	include "Core/LanguageHelpers/IStack/Build-IStack.lua"
	include "Core/LanguageHelpers/ITokeniser/Build-ITokeniser.lua"
group "Core/Common"
	include "Core/Logger/Build-Logger.lua"
	include "Core/Profiler/Build-Profiler.lua"
	include "Core/SmartEnums/Build-SmartEnums.lua"
group "Core/Interpreters"
	include "Core/Interpreters/BrainFckInterpreter/Build-BrainFckInterpreter.lua"
	include "Core/Interpreters/NullInterpreter/Build-NullInterpreter.lua"
	include "Core/Interpreters/PietInterpreter/Build-PietInterpreter.lua"
group "App"
	include "App/Build-EsoProg.lua"
group ""

