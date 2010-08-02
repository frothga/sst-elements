// Copyright 2009-2010 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
// 
// Copyright (c) 2009-2010, Sandia Corporation
// All rights reserved.
// 
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#ifndef _INTROSPECTOR_CPU_H
#define _INTROSPECTOR_CPU_H

#include <sst/core/introspector.h>


using namespace SST;

#if DBG_INTROSPECTOR_CPU
#define _INTROSPECTOR_CPU_DBG( fmt, args...)\
         printf( "%d:Introspector_cpu::%s():%d: "fmt, _debug_rank, __FUNCTION__,__LINE__, ## args )
#else
#define _INTROSPECTOR_CPU_DBG( fmt, args...)
#endif



class Introspector_cpu : public Introspector {
       
    public:
	Introspector_cpu(Component::Params_t& params ) :
            Introspector(),
            params( params ),
            frequency( "1ns" )
        {
            _INTROSPECTOR_CPU_DBG( "new id=%lu\n", id );

            Component::Params_t::iterator it = params.begin(); 
            while( it != params.end() ) { 
                _INTROSPECTOR_CPU_DBG("key=%s value=%s\n",
                            it->first.c_str(),it->second.c_str());
                if ( ! it->first.compare("period") ) {
		    frequency = it->second;
                }  
		else if ( ! it->first.compare("model") ) {
		    model = it->second;
                }    
                ++it;
            } 
            
            intData = 0;
            _INTROSPECTOR_CPU_DBG("-->frequency=%s\n",frequency.c_str());
//             handler = new EventHandler< Introspector_cpu, bool, Cycle_t >
//                                                 ( this, &Introspector_cpu::pullData );
//             TimeConverter* tc = registerClock( frequency, handler );
	    TimeConverter* tc = registerClock( frequency, new Clock::Handler<Introspector_cpu>(this, &Introspector_cpu::pullData) );

// 	     mpihandler = new EventHandler< Introspector_cpu, bool, Cycle_t >
// 	                                        ( this, &Introspector_cpu::mpiCollectInt );
// 	     registerClock( frequency, mpihandler );
	    //FIXME: GH
	    /*registerClock( frequency, new Clock::Handler<Introspector_cpu>(this, &Introspector_cpu::mpiCollectInt) );*/
	    //FIXME: GH
	    /*mpionetimehandler = new Event::Handler< Introspector_cpu >
	                                        ( this, &Introspector_cpu::mpiOneTimeCollect );*/

	    printf("INTROSPECTOR_CPU period: %ld\n",(long int)tc->getFactor());
            _INTROSPECTOR_CPU_DBG("Done registering clock\n");
            
        }
        int Setup() {
	    std::pair<bool, int> pint;
	    std::pair<bool, double*> pdouble;

	    //get a list of relevant component. Must be done after all components are created 
	    MyCompList = getModels(model); 
	    //std::cout << " introspector_cpu has MyCompList size = " << MyCompList.size() << std::endl;
	    for (std::list<IntrospectedComponent*>::iterator i = MyCompList.begin();
	        i != MyCompList.end(); ++i) {
     		    // state that we will monitor those components 
		    // (pass introspector's info to the component)
     		    monitorComponent(*i);

		    //check if the component counts the specified int/double data
		    //pint = (*i)->ifMonitorIntData("core_temperature");
		    //pint = (*i)->ifMonitorIntData("branch_read");
		    //pint = (*i)->ifMonitorIntData("branch_write");
		    //pint = (*i)->ifMonitorIntData("RAS_read");
		    //pint = (*i)->ifMonitorIntData("RAS_write");
		    pint = (*i)->ifMonitorIntData("il1_read");
		    //pdouble = (*i)->ifMonitorDoubleData("CPUarea");

		    if(pint.first){
			//store pointer to component and the dataID of the data of interest
			//std::cout << "introspector_cpu is calling addToIntDatabase." << std::endl;
			addToIntDatabase(*i, pint.second);
			//std::cout << " introspector_cpu now has intdatabase size = " << DatabaseInt.size() << std::endl;
		    }
		    //if(pdouble.first){
		        //store pointer to component and the dataID of the data of interest
			//addToDoubleDatabase(*i, pdouble.second);
		    //}


	     }
	    oneTimeCollect(90000, mpionetimehandler);
            _INTROSPECTOR_CPU_DBG("\n");
            return 0;
        }
        int Finish() {
            _INTROSPECTOR_CPU_DBG("\n");
            return 0;
        }


    private:
        Introspector_cpu( const Introspector_cpu& c );
	//~Introspector_cpu();
	Introspector_cpu() :  Introspector() {} // for serialization only

        bool pullData( Cycle_t );
	bool mpiCollectInt( Cycle_t );
	void mpiOneTimeCollect(Event* e);

	Event::Handler< Introspector_cpu > *mpionetimehandler;
        Component::Params_t    params;        
	std::string frequency;
	std::string model;
	uint64_t intData;

	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version )
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Introspector);
        ar & BOOST_SERIALIZATION_NVP(params);
        ar & BOOST_SERIALIZATION_NVP(frequency);
	ar & BOOST_SERIALIZATION_NVP(model);
    }
/*	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);*/
};

#endif
