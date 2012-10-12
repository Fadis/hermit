#ifndef HERMIT_HTTP_TRANSFER_ENCODING_HPP
#define HERMIT_HTTP_TRANSFER_ENCODING_HPP

#include <string>
#include <vector>
#include <map>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

namespace hermit {
  namespace http {
    struct transfer_encoding {
      typedef std::map< std::string, boost::optional< std::string > > attribute_type;
      transfer_encoding() : {}
      std::string name;
      attribute_type attribute;
    };
  }
}
BOOST_FUSION_ADAPT_STRUCT(
  hermit::http::transfer_encoding,
  (std::string,name)
  (hermit::http::transfer_encoding::attribute_type,attribute)
)

#endif

