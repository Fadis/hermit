#ifndef HERMIT_INT128_T_HPP
#define HERMIT_INT128_T_HPP

#if defined( __GNUC__ ) && !defined( __clang__ )
# if ( __GNUC__ == 4 && __GNUC_MINOR__ >= 2 )
#  if (defined(__LP64__) || defined(__x86_64__) || defined(__powerpc64__))

#define HAVE_INT128_T

namespace hermit {
  typedef __int128_t int128_t;
  typedef __uint128_t uint128_t;
}

#  endif
# endif
#endif

#ifdef __clang__
# if __clang_major__ >= 3
#  if (defined(__LP64__) || defined(__x86_64__) || defined(__powerpc64__))

#define HAVE_INT128_T
namespace hermit {
  typedef __int128_t int128_t;
  typedef __uint128_t uint128_t;
}

#  endif
# endif
#endif

#endif
