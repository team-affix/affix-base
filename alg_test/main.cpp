#include "affix_base.h"

int main() {

	using affix_base::data::builder;

	builder b(8);

	while (!(b << 0));

	uint64_t u = b.build<uint64_t>();

	return EXIT_SUCCESS;
}