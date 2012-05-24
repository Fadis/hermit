
ADD_LIBRARY( hermit_mpint SHARED mpint.cpp )
target_link_libraries( hermit_mpint ${TomMath_LIBRARY} )
ADD_LIBRARY( hermit_ip SHARED ip.cpp )
target_link_libraries( hermit_ip hermit_mpint )
set_target_properties( hermit_ip PROPERTIES
  VERSION 0.0.1
  SOVERSION 0.0.1
  )
ADD_EXECUTABLE( ip_sample ip_sample.cpp )
target_link_libraries( ip_sample hermit_ip )

install( TARGETS hermit_mpint hermit_ip
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  )
