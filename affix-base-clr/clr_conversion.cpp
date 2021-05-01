#include "clr_conversion.h"

using namespace affix_base;

void* conversion::unmanaged(System::Object^ a_obj) {
	return GCHandle::ToIntPtr(GCHandle::Alloc(a_obj)).ToPointer();
}

System::Object^ conversion::managed(void* a_ptr) {
	return GCHandle::FromIntPtr(IntPtr(a_ptr)).Target;
}

GCHandle^ conversion::handle(void* a_ptr) {
	return GCHandle::FromIntPtr(IntPtr(a_ptr));
}