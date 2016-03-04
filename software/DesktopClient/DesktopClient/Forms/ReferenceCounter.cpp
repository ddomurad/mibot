#include "ReferenceCounter.h"

ReferenceCounter::ReferenceCounter()
{
    _ref_counter++;
}

ReferenceCounter::~ReferenceCounter()
{
    _ref_counter--;
}

int ReferenceCounter::GetRefCount()
{
    return _ref_counter;
}


int ReferenceCounter::_ref_counter = 0;
