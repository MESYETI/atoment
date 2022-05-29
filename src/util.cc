#include "util.hh"

bool Util::IsNumber(std::string str) {
	if (str == "-") {
		return false;
	}
	if (str == "") {
		return false;
	}
	for (size_t i = 0; i<str.length(); ++i) {
		if (!(
			((str[i] >= '0') && (str[i] <= '9')) ||
			((i == 0) && (str[i] == '-'))
		)) {
			return false;
		}
	}
	return true;
}
