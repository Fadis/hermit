#include <iostream>
#include <vector>
#include <list>

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <hermit/has_member_function_xxx.hpp>
#include <hermit/has_function_xxx.hpp>
#include <hermit/iterator_traits.hpp>
#include <boost/range.hpp>
#include <boost/iterator/is_readable_iterator.hpp>
#include <boost/iterator/is_lvalue_iterator.hpp>
#include <hermit/range_traits.hpp>

int main() {
  std::cout << "s f b r|r w s l" << std::endl;
  std::cout << hermit::is_single_pass_traversal_range< std::vector< int > >::value << " ";
  std::cout << hermit::is_forward_traversal_range< std::vector< int > >::value << " ";
  std::cout << hermit::is_bidirectional_traversal_range< std::vector< int > >::value << " ";
  std::cout << hermit::is_random_access_traversal_range< std::vector< int > >::value << " ";
  std::cout << hermit::is_readable_range< std::vector< int > >::value << " ";
  std::cout << hermit::is_writable_range< std::vector< int >, int >::value << " ";
  std::cout << hermit::is_swappable_range< std::vector< int > >::value << " ";
  std::cout << hermit::is_lvalue_range< std::vector< int > >::value << std::endl;

  std::cout << hermit::is_single_pass_traversal_range< std::list< int > >::value << " ";
  std::cout << hermit::is_forward_traversal_range< std::list< int > >::value << " ";
  std::cout << hermit::is_bidirectional_traversal_range< std::list< int > >::value << " ";
  std::cout << hermit::is_random_access_traversal_range< std::list< int > >::value << " ";
  std::cout << hermit::is_readable_range< std::list< int > >::value << " ";
  std::cout << hermit::is_writable_range< std::list< int >, int >::value << " ";
  std::cout << hermit::is_swappable_range< std::list< int > >::value << " ";
  std::cout << hermit::is_lvalue_range< std::list< int > >::value << std::endl;

  std::cout << hermit::is_single_pass_traversal_range< std::vector< bool > >::value << " ";
  std::cout << hermit::is_forward_traversal_range< std::vector< bool > >::value << " ";
  std::cout << hermit::is_bidirectional_traversal_range< std::vector< bool > >::value << " ";
  std::cout << hermit::is_random_access_traversal_range< std::vector< bool > >::value << " ";
  std::cout << hermit::is_readable_range< std::vector< bool > >::value << " ";
  std::cout << hermit::is_writable_range< std::vector< bool >, bool >::value << " ";
  std::cout << hermit::is_swappable_range< std::vector< bool > >::value << " ";
  std::cout << hermit::is_lvalue_range< std::vector< bool > >::value << std::endl;

  std::cout << hermit::is_single_pass_traversal_range< std::pair< int*, int* > >::value << " ";
  std::cout << hermit::is_forward_traversal_range< std::pair< int*, int* > >::value << " ";
  std::cout << hermit::is_bidirectional_traversal_range< std::pair< int*, int* > >::value << " ";
  std::cout << hermit::is_random_access_traversal_range< std::pair< int*, int* > >::value << " ";
  std::cout << hermit::is_readable_range< std::pair< int*, int* > >::value << " ";
  std::cout << hermit::is_writable_range< std::pair< int*, int* >, int >::value << " ";
  std::cout << hermit::is_swappable_range< std::pair< int*, int* > >::value << " ";
  std::cout << hermit::is_lvalue_range< std::pair< int*, int* > >::value << std::endl;
  
  std::cout << "misc" << std::endl;

  std::cout << hermit::is_single_pass_traversal_range< int >::value << std::endl;
  std::cout << hermit::is_forward_traversal_range< int >::value << std::endl;
  std::cout << hermit::is_bidirectional_traversal_range< int >::value << std::endl;
  std::cout << hermit::is_random_access_traversal_range< int >::value << std::endl;
}

