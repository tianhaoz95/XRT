add_subdirectory(include)
add_subdirectory(common)
 if (${XRT_NATIVE_BUILD} STREQUAL "yes")
   add_subdirectory(pcie)
   if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
     # Emulation flow only works on x86_64
     add_subdirectory(emulation)
   endif()
 else()
   add_subdirectory(edge)
 endif()
