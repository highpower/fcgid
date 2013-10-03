#include "acsetup.hpp"
#include "fastcgi-daemon/details/urltree.hpp"

namespace fcgid { namespace details {

range<char const*> const
urltree_node_base::wildcard_token = make_range("*");

}} // namespaces
