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
#include <hermit/has_member_accessible_xxx.hpp>
#include <hermit/has_member_type_xxx.hpp>
#include <hermit/has_member_variable_xxx.hpp>
#include <hermit/has_member_function_xxx.hpp>
#include <hermit/has_function_xxx.hpp>


HPP_HAS_MEMBER_ACCESSIBLE_XXX( neo_detect_hoge, hoge )
HPP_HAS_MEMBER_TYPE_XXX( detect_type_hoge, hoge )
HPP_HAS_MEMBER_VARIABLE_XXX( detect_value_hoge, hoge )
HPP_HAS_MEMBER_FUNCTION_XXX( detect_function_hoge, hoge )
HPP_HAS_FUNCTION_XXX( detect_adl_function_hoge, hoge )

struct A {};
struct B { void hoge( float*, float* ); };
struct C { int hoge; };
struct D { typedef int hoge; };
struct E { struct hoge {}; };
class F { void hoge( float*, float* ); };
class G { int hoge; };
class H { typedef int hoge; };
class I { struct hoge {}; };
typedef B J;
typedef C K;
typedef D L;
typedef E M;
struct N : public B {};
struct O : public C {};
struct P : public D {};
struct Q : public E {};
struct R { static void hoge( float*, float* ); };

namespace foo {
  struct bar{};
  void hoge( bar );
}

int main() {
  std::cout << neo_detect_hoge< A >::value << " ";
  std::cout << neo_detect_hoge< B >::value << " ";
  std::cout << neo_detect_hoge< C >::value << " ";
  std::cout << neo_detect_hoge< D >::value << " ";
  std::cout << neo_detect_hoge< E >::value << " ";
#if __GNUC__ && !__clang__
  std::cout << "x ";
  std::cout << "x ";
  std::cout << "x ";
  std::cout << "x ";
#else
  std::cout << neo_detect_hoge< F >::value << " ";
  std::cout << neo_detect_hoge< G >::value << " ";
  std::cout << neo_detect_hoge< H >::value << " ";
  std::cout << neo_detect_hoge< I >::value << " ";
#endif
  std::cout << neo_detect_hoge< J >::value << " ";
  std::cout << neo_detect_hoge< K >::value << " ";
  std::cout << neo_detect_hoge< L >::value << " ";
  std::cout << neo_detect_hoge< M >::value << " ";
  std::cout << neo_detect_hoge< N >::value << " ";
  std::cout << neo_detect_hoge< O >::value << " ";
  std::cout << neo_detect_hoge< P >::value << " ";
  std::cout << neo_detect_hoge< Q >::value << " ";
  std::cout << neo_detect_hoge< R >::value << " ";
  std::cout << neo_detect_hoge< int >::value << std::endl;

  std::cout << detect_type_hoge< A >::value << " ";
  std::cout << detect_type_hoge< B >::value << " ";
  std::cout << detect_type_hoge< C >::value << " ";
  std::cout << detect_type_hoge< D >::value << " ";
  std::cout << detect_type_hoge< E >::value << " ";
#if __GNUC__ && !__clang__
  std::cout << "x ";
  std::cout << "x ";
  std::cout << "x ";
  std::cout << "x ";
#else
  std::cout << detect_type_hoge< F >::value << " ";
  std::cout << detect_type_hoge< G >::value << " ";
  std::cout << detect_type_hoge< H >::value << " ";
  std::cout << detect_type_hoge< I >::value << " ";
#endif
  std::cout << detect_type_hoge< J >::value << " ";
  std::cout << detect_type_hoge< K >::value << " ";
  std::cout << detect_type_hoge< L >::value << " ";
  std::cout << detect_type_hoge< M >::value << " ";
  std::cout << detect_type_hoge< N >::value << " ";
  std::cout << detect_type_hoge< O >::value << " ";
  std::cout << detect_type_hoge< P >::value << " ";
  std::cout << detect_type_hoge< Q >::value << " ";
  std::cout << detect_type_hoge< R >::value << " ";
  std::cout << detect_type_hoge< int >::value << std::endl;

  std::cout << detect_value_hoge< A, int >::value << " ";
  std::cout << detect_value_hoge< B, int >::value << " ";
  std::cout << detect_value_hoge< C, int >::value << " ";
  std::cout << detect_value_hoge< D, int >::value << " ";
  std::cout << detect_value_hoge< E, int >::value << " ";
#if __GNUC__ && !__clang__
  std::cout << "x ";
  std::cout << "x ";
  std::cout << "x ";
  std::cout << "x ";
#else
  std::cout << detect_value_hoge< F, int >::value << " ";
  std::cout << detect_value_hoge< G, int >::value << " ";
  std::cout << detect_value_hoge< H, int >::value << " ";
  std::cout << detect_value_hoge< I, int >::value << " ";
#endif
  std::cout << detect_value_hoge< J, int >::value << " ";
  std::cout << detect_value_hoge< K, int >::value << " ";
  std::cout << detect_value_hoge< L, int >::value << " ";
  std::cout << detect_value_hoge< M, int >::value << " ";
  std::cout << detect_value_hoge< N, int >::value << " ";
  std::cout << detect_value_hoge< O, int >::value << " ";
  std::cout << detect_value_hoge< P, int >::value << " ";
  std::cout << detect_value_hoge< Q, int >::value << " ";
  std::cout << detect_value_hoge< R, int >::value << " ";
  std::cout << detect_value_hoge< int, int >::value << std::endl;


  std::cout << detect_function_hoge< A, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< B, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< C, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< D, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< E, void( float*, float* ) >::value << " ";
#if __GNUC__ && !__clang__
  std::cout << "x ";
  std::cout << "x ";
  std::cout << "x ";
  std::cout << "x ";
#else
  std::cout << detect_function_hoge< F, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< G, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< H, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< I, void( float*, float* ) >::value << " ";
#endif
  std::cout << detect_function_hoge< J, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< K, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< L, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< M, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< N, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< O, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< P, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< Q, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< R, void( float*, float* ) >::value << " ";
  std::cout << detect_function_hoge< int, void( float*, float* ) >::value << std::endl;

  std::cout << detect_adl_function_hoge< void( foo::bar ) >::value << std::endl;
}

