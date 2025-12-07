project "SmartEnums"
   kind "None"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"

   files { "Src/**.h" }

   includedirs
   {
   }

   targetdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir)
   objdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir .. "/Intermediates")
