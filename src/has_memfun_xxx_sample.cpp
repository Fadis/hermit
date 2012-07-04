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
#include <hermit/has_member_xxx.hpp>

HPP_HAS_MEMBER_XXX( detect_hoge, hoge )

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
 
int main() {
  std::cout << "detect_hoge< T >" << std::endl;
  std::cout << detect_hoge< A >::value << " ";
  std::cout << detect_hoge< B >::value << " ";
  std::cout << detect_hoge< C >::value << " ";
  std::cout << detect_hoge< D >::value << " ";
  std::cout << detect_hoge< E >::value << " ";
#if __GNUC__ == 4 && __GNUC_MINOR__ == 7
  std::cout << 'b' << " ";
  std::cout << 'b' << " ";
  std::cout << 'b' << " ";
  std::cout << 'b' << " ";
#else
  std::cout << detect_hoge< F >::value << " ";
  std::cout << detect_hoge< G >::value << " ";
  std::cout << detect_hoge< H >::value << " ";
  std::cout << detect_hoge< I >::value << " ";
#endif
  std::cout << detect_hoge< J >::value << " ";
  std::cout << detect_hoge< K >::value << " ";
  std::cout << detect_hoge< L >::value << " ";
  std::cout << detect_hoge< M >::value << " ";
  std::cout << detect_hoge< N >::value << " ";
  std::cout << detect_hoge< O >::value << " ";
  std::cout << detect_hoge< P >::value << " ";
  std::cout << detect_hoge< Q >::value << " ";
  std::cout << detect_hoge< R >::value << " ";
  std::cout << detect_hoge< int >::value << std::endl;
  std::cout << "detect_hoge< T, int >" << std::endl;
  std::cout << detect_hoge< A, int >::value << " ";
  std::cout << detect_hoge< B, int >::value << " ";
  std::cout << detect_hoge< C, int >::value << " ";
  std::cout << detect_hoge< D, int >::value << " ";
  std::cout << detect_hoge< E, int >::value << " ";
#if __GNUC__ == 4 && __GNUC_MINOR__ == 7
  std::cout << 'b' << " ";
  std::cout << 'b' << " ";
  std::cout << 'b' << " ";
  std::cout << 'b' << " ";
#else
  std::cout << detect_hoge< F, int >::value << " ";
  std::cout << detect_hoge< G, int >::value << " ";
  std::cout << detect_hoge< H, int >::value << " ";
  std::cout << detect_hoge< I, int >::value << " ";
#endif
  std::cout << detect_hoge< J, int >::value << " ";
  std::cout << detect_hoge< K, int >::value << " ";
  std::cout << detect_hoge< L, int >::value << " ";
  std::cout << detect_hoge< M, int >::value << " ";
  std::cout << detect_hoge< N, int >::value << " ";
  std::cout << detect_hoge< O, int >::value << " ";
  std::cout << detect_hoge< P, int >::value << " ";
  std::cout << detect_hoge< Q, int >::value << " ";
  std::cout << detect_hoge< R, int >::value << " ";
  std::cout << detect_hoge< int, int >::value << std::endl;
  std::cout << "detect_hoge< T, void( float*, float* ) >" << std::endl;
  std::cout << detect_hoge< A, void( float*, float* ) >::value << " ";
#if __GNUC__ && !__clang__
  std::cout << 'b' << " ";
#else
  std::cout << detect_hoge< B, void( float*, float* ) >::value << " ";
#endif
  std::cout << detect_hoge< C, void( float*, float* ) >::value << " ";
  std::cout << detect_hoge< D, void( float*, float* ) >::value << " ";
  std::cout << detect_hoge< E, void( float*, float* ) >::value << " ";
#if __GNUC__ && !__clang__
  std::cout << 'b' << " ";
  std::cout << 'b' << " ";
  std::cout << 'b' << " ";
  std::cout << 'b' << " ";
#else
#if __GNUC__ == 4 && __GNUC_MINOR__ == 7
  std::cout << 'b' << " ";
  std::cout << 'b' << " ";
#else
  std::cout << detect_hoge< F, void( float*, float* ) >::value << " ";
  std::cout << detect_hoge< G, void( float*, float* ) >::value << " ";
#endif
  std::cout << detect_hoge< H, void( float*, float* ) >::value << " ";
  std::cout << detect_hoge< I, void( float*, float* ) >::value << " ";
#endif
#if __GNUC__ && !__clang__
  std::cout << 'b' << " ";
#else
  std::cout << detect_hoge< J, void( float*, float* ) >::value << " ";
#endif
  std::cout << detect_hoge< K, void( float*, float* ) >::value << " ";
  std::cout << detect_hoge< L, void( float*, float* ) >::value << " ";
  std::cout << detect_hoge< M, void( float*, float* ) >::value << " ";
#if __GNUC__ && !__clang__
  std::cout << 'b' << " ";
#else
  std::cout << detect_hoge< N, void( float*, float* ) >::value << " ";
#endif
  std::cout << detect_hoge< O, void( float*, float* ) >::value << " ";
  std::cout << detect_hoge< P, void( float*, float* ) >::value << " ";
  std::cout << detect_hoge< Q, void( float*, float* ) >::value << " ";
  std::cout << detect_hoge< R, void( float*, float* ) >::value << " ";
  std::cout << detect_hoge< int, void( float*, float* ) >::value << std::endl;

}

