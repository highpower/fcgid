#include "acsetup.hpp"
#include "dynamic_loader.hpp"

#include <list>

#include "ltdl.h"
#include "fastcgi-daemon/details/resource.hpp"

namespace fcgid { namespace main {

class dynamic_loader_impl : public dynamic_loader {

public:
	dynamic_loader_impl();
	virtual ~dynamic_loader_impl();
	virtual void load(char const *module);

private:
	dynamic_loader_impl(dynamic_loader_impl const &);
	dynamic_loader_impl& operator = (dynamic_loader_impl const &);

private:
	std::list<lt_dlhandle> modules_;
};

struct lt_dlhandle_traits {
	void destroy(lt_dlhandle h);
	static lt_dlhandle default_value();
};

dynamic_loader::dynamic_loader()
{
}

dynamic_loader::~dynamic_loader() {
}

std::shared_ptr<dynamic_loader>
create_loader() {
	return std::shared_ptr<dynamic_loader>(new dynamic_loader_impl());
}

dynamic_loader_impl::dynamic_loader_impl()
{
}

dynamic_loader_impl::~dynamic_loader_impl() {
	for (std::list<lt_dlhandle>::iterator i = modules_.begin(), end = modules_.end(); i != end; ++i) {
		lt_dlclose(*i);
	}
}

void
dynamic_loader_impl::load(char const *module) {
	details::resource<lt_dlhandle, lt_dlhandle_traits> handle;
	handle.reset(lt_dlopen(module));
}

void
lt_dlhandle_traits::destroy(lt_dlhandle h) {
	lt_dlclose(h);
}

lt_dlhandle
lt_dlhandle_traits::default_value() {
	return static_cast<lt_dlhandle>(0);
}

}} // namespaces
