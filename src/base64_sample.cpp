#include <string>
#include <algorithm>
#include <iostream>
#include <hermit/format/read/base64.hpp>
#include <hermit/format/write/base64.hpp>

int main() {
  const std::string source(
  "I2luY2x1ZGUgPHN0cmluZz4KI2luY2x1ZGUgPGlvc3RyZWFtPgojaW5jbHVkZSA8aGVybWl0L3Nw\n"
  "aXJpdC91dGY4LmhwcD4KCmludCBtYWluKCkgewogIHN0ZDo6c3RyaW5nIHNvdXJjZSggIuOCs+OD\n"
  "s+ODkeOCpOODq+aZgumWk+OBjOOBruOBs+OBruOBs+ODvCIgKTsKICBzdGQ6OnUzMnN0cmluZyBk\n"
  "ZWNvZGVkOwogIHsKICAgIGhlcm1pdDo6c3Bpcml0OjpxaTo6dXRmODwgc3RkOjpzdHJpbmc6Oml0\n"
  "ZXJhdG9yID4gcnVsZTsKICAgIGF1dG8gaXRlciA9IHNvdXJjZS5iZWdpbigpOwogICAgYm9vc3Q6\n"
  "OnNwaXJpdDo6cWk6OnBhcnNlKCBpdGVyLCBzb3VyY2UuZW5kKCksICpydWxlLCBkZWNvZGVkICk7\n"
  "CiAgfQogIGZvciggYXV0byAmZWxlbTogZGVjb2RlZCApCiAgICBzdGQ6OmNvdXQgPDwgc3RhdGlj\n"
  "X2Nhc3Q8IHVpbnQzMl90ID4oIGVsZW0gKSA8PCAnLCc7CiAgc3RkOjpjb3V0IDw8IHN0ZDo6ZW5k\n"
  "bDsKICBzdGQ6OnN0cmluZyBlbmNvZGVkOwogIHsKICAgIHR5cGVkZWYgc3RkOjpiYWNrX2luc2Vy\n"
  "dF9pdGVyYXRvcjxzdGQ6OnN0cmluZz4gc2lua190eXBlOwogICAgaGVybWl0OjpzcGlyaXQ6Omth\n"
  "cm1hOjp1dGY4PCBzaW5rX3R5cGUgPiBydWxlOwogICAgYm9vc3Q6OnNwaXJpdDo6a2FybWE6Omdl\n"
  "bmVyYXRlKCBzaW5rX3R5cGUoIGVuY29kZWQgKSwgKnJ1bGUsIGRlY29kZWQgKTsKICB9CiAgc3Rk\n"
  "Ojpjb3V0IDw8IGVuY29kZWQgPDwgc3RkOjplbmRsOwp9Cgo="
      );
  const auto decoded = hermit::format::read_base64( source );
  if( !decoded ) {
    std::cerr << "Invalid inputs." << std::endl;
    abort();
  }
  for( const auto elem : *decoded )
    std::cout << elem;
  std::cout << std::endl;
  const auto encoded = hermit::format::write_base64( *decoded );
  if( !encoded ) {
    std::cerr << "Invalid inputs. :" << std::endl;
    abort();
  }
  std::cout << *encoded << std::endl;
}

