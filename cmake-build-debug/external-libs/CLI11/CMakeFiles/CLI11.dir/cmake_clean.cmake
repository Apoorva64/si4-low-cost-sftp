file(REMOVE_RECURSE
  "libCLI11.a"
  "libCLI11.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/CLI11.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
