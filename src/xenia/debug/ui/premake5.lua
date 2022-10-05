project_root = "../../../.."
include(project_root.."/tools/build")

group("src")
project("xenia-debug-ui")
  uuid("9193a274-f4c2-4746-bd85-93fcfc5c3e38")
  kind("StaticLib")
  language("C++")
  links({
    "imgui",
    "xenia-base",
    "xenia-cpu",
    "xenia-ui",
  })
  filter({"configurations:Release", "platforms:Windows"})
    buildoptions({
      "/Os",
      "/O1"
    })
  defines({
  })
  includedirs({
  })
  local_platform_files()
