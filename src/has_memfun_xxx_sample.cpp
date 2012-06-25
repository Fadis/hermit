#include <iostream>
#include <vector>
#include <boost/preprocessor.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/member_function_pointer.hpp>
#include <boost/function_types/components.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>
#include <cxxabi.h>
#include <hermit/has_memfun_xxx.hpp>


HPP_MEMFUN_XXX( assign )
   
class Moo {};
 
int main() {
  std::cout << detect_assign< std::vector< int >, void( int*, int* ) >::value << std::endl;
  std::cout << detect_assign< Moo, void( int*, int* ) >::value << std::endl;
}
