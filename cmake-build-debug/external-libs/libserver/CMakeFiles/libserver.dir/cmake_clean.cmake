file(REMOVE_RECURSE
  "liblibserver.a"
  "liblibserver.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/libserver.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
