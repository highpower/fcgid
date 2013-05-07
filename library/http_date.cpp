#include "acsetup.hpp"
#include "fastcgi-daemon/http_date.hpp"

#include <limits>
#include <cerrno>
#include <cstdio>

#include "fastcgi-daemon/details/runtime_error.hpp"

namespace fcgid {

http_date const
http_date::bad(std::numeric_limits<std::time_t>::max());

static char const*
date_formats[] = { 
	"%a, %d %b %Y %T GMT", "%A, %d-%b-%y %T GMT", "%a %b %d %T %Y" 
};

static char const*
short_month_names[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static char const*
long_weekday_names[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

static char const*
short_weekday_names[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

http_date::http_date() :
	when_(time(0))
{
}

http_date::http_date(std::time_t t) :
	when_(t)
{
}

void
http_date::swap(http_date &other) throw () {
	using std::swap;
	swap(when_, other.when_);
}

http_date
http_date::from_now(char const *value) {
	http_date date;
	return date;
}

std::size_t
http_date::rfc1123(char *buffer, std::size_t size) const {

	using namespace details;
	
	struct tm ts;
	if (0 == gmtime_r(&when_, &ts)) {
		throw runtime_error("date <%llu> format failed", static_cast<unsigned long long>(when_));
	}

	int res = snprintf(buffer, size - 1, "%s, %02d %s %04d %02d:%02d:%02d GMT",
		short_weekday_names[ts.tm_wday], ts.tm_mday, short_month_names[ts.tm_mon], ts.tm_year + 1900,
		ts.tm_hour, ts.tm_min, ts.tm_sec);

	if (res < 0) {
		throw runtime_error("date <%llu> format failed with error code %d", 
			static_cast<unsigned long long>(when_), errno);
	}
	
	return res;
}

std::size_t
http_date::rfc1036(char *buffer, std::size_t size) const {

    using namespace details;

	struct tm ts;
	if (0 == gmtime_r(&when_, &ts)) {
		throw runtime_error("date <%llu> format failed", static_cast<unsigned long long>(when_));
	}

	int res = snprintf(buffer, size - 1, "%s, %02d-%s-%02d %02d:%02d:%02d GMT",
		long_weekday_names[ts.tm_wday], ts.tm_mday, short_month_names[ts.tm_mon], ts.tm_year % 100,
		ts.tm_hour, ts.tm_min, ts.tm_sec);

	if (res < 0) {
		throw runtime_error("date <%llu> format failed with error code %d", 
			static_cast<unsigned long long>(when_), errno);
	}

	return res;
}

std::size_t
http_date::asctime(char *buffer, std::size_t size) const {

    using namespace details;

	struct tm ts;
	if (0 == gmtime_r(&when_, &ts)) {
		throw runtime_error("date <%llu> format failed", static_cast<unsigned long long>(when_));
	}

	int res = snprintf(buffer, size - 1, "%s %s %d %02d:%02d:%02d %04d", 
		short_weekday_names[ts.tm_wday], short_month_names[ts.tm_mon], ts.tm_mday, 
		ts.tm_hour, ts.tm_min, ts.tm_sec, ts.tm_year + 1900);

	if (res < 0) {
		throw runtime_error("date <%llu> format failed with error code %d", 
			static_cast<unsigned long long>(when_), errno);
	}
	
	return res;
}

} // namespace
