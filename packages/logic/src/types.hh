#pragma once
#include "safe_id.hh"

struct LocationTag;
using LocationID = SafeID<uint16_t, struct LocationTag>;
