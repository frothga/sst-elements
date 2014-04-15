// Copyright 2009-2014 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2014, Sandia Corporation
// All rights reserved.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.


#ifndef _H_SST_ARIEL_FREE_EVENT
#define _H_SST_ARIEL_FREE_EVENT

#include "arielevent.h"

using namespace SST;

namespace SST {
namespace ArielComponent {

class ArielFreeEvent : public ArielEvent {

	public:
		ArielFreeEvent(uint64_t vAddr);
		~ArielFreeEvent();
		ArielEventType getEventType();
		uint64_t getVirtualAddress();

	protected:
		uint64_t virtualAddress;

};

}
}

#endif
