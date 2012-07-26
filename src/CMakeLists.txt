
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
ADD_EXECUTABLE( uri_sample uri_sample.cpp )
target_link_libraries( uri_sample hermit_ip )
target_link_libraries( uri_sample ${Boost_LIBRARIES} )
ADD_EXECUTABLE( simd_sample simd_sample.cpp )
ADD_EXECUTABLE( has_memfun_xxx_sample has_memfun_xxx_sample.cpp )
ADD_EXECUTABLE( range_traits_sample range_traits_sample.cpp )
ADD_EXECUTABLE( s_expr_sample s_expr_sample.cpp )
ADD_EXECUTABLE( json_sample json_sample.cpp )
ADD_EXECUTABLE( delayed_sample delayed_sample.cpp )
target_link_libraries( delayed_sample ${Boost_LIBRARIES} )
ADD_EXECUTABLE( task_remapper_sample task_remapper_sample.cpp )
target_link_libraries( task_remapper_sample ${Boost_LIBRARIES} )

install( TARGETS hermit_mpint hermit_ip
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  )
