#include "mutex.h"

using affix::threading::mutex;

mutex::mutex() {

}

void mutex::lock() {
	size_t l_queue = queue;
	queue++;
	while (l_queue - queue > 1);
}

void mutex::unlock() {
	queue--;
}