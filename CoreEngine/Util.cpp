#include "Util.h"

char * Util::charVec2cStr(std::vector<char> in)
{
	char *c = new char[in.size() + 1];
	memcpy(c, in.data(), in.size());
	c[in.size()] = 0;

	return c;
}
