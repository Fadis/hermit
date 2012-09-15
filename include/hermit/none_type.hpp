#ifndef HERMIT_NONE_TYPE_HPP
#define HERMIT_NONE_TYPE_HPP

namespace hermit {
  struct none_type {
    bool operator==( const none_type &src ) const { return true; }
    bool operator!=( const none_type &src ) const { return false; }
    bool operator>( const none_type &src ) const { return false; }
    bool operator<( const none_type &src ) const { return false; }
    bool operator>=( const none_type &src ) const { return true; }
    bool operator<=( const none_type &src ) const { return true; }
  };
}

#endif

