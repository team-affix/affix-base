#include "pch.h"
#include "ptr.h"

using affix_base::data::ptr_base;
using std::vector;
using std::map;
using namespace affix_base::threading;

guarded_resource<map<void*, std::vector<ptr_base*>>, cross_thread_mutex> ptr_base::s_res_map;
