#pragma once

#include "FLIST.h"

// Element functions take a real, non-null node, not the sentinel.
flist::TInfo find_max(flist::ptrNODE ptr);
void reverse_print(flist::ptrNODE ptr);
bool replace_all(flist::ptrNODE ptr, flist::TInfo old_el, flist::TInfo new_el);
// before is the predecessor of a real node; it may be the sentinel.
bool delete_first(flist::FLIST& list, flist::ptrNODE before, flist::TInfo elem);
bool duplicate_first(flist::FLIST& list, flist::ptrNODE ptr, flist::TInfo elem);
bool duplicate_all(flist::FLIST& list, flist::ptrNODE ptr, flist::TInfo elem);
bool is_increasing(flist::ptrNODE ptr);
