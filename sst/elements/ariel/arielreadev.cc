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


#include <sst_config.h>
#include "arielreadev.h"

using namespace SST::ArielComponent;

ArielReadEvent::ArielReadEvent(uint64_t rAddr, uint32_t len) {
	readAddress = rAddr;
	readLength = len;
}

ArielReadEvent::~ArielReadEvent() {

}

ArielEventType ArielReadEvent::getEventType() {
	return READ_ADDRESS;
}

uint64_t ArielReadEvent::getAddress() {
	return readAddress;
}

uint32_t ArielReadEvent::getLength() {
	return readLength;
}
