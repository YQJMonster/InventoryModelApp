# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\InventoryModel_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\InventoryModel_autogen.dir\\ParseCache.txt"
  "InventoryModel_autogen"
  )
endif()
