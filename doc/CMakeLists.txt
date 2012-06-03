#add a target for documentation generation
find_package(Doxygen)
if(DOXYGEN_FOUND)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} Doxyfile ALL
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)
