ADD_EXECUTABLE( sample sample.cpp ip.cpp )
target_link_libraries( sample ${TomMath_LIBRARY} )
install( TARGETS sample
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)
