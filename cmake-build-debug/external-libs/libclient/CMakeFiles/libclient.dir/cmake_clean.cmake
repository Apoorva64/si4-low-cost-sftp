file(REMOVE_RECURSE
  "liblibclient.a"
  "liblibclient.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/libclient.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
