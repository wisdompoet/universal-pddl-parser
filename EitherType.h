
#ifndef _EITHERTYPE_H_
#define _EITHERTYPE_H_

#include "Type.h"

class EitherType : public Type {

public:

	EitherType( const std::string & s )
		: Type( s ) {
	}
	
	std::string getName() const {
		return "( EITHER " + subtypes[0]->name + " " + subtypes[1]->name + " )";
	}

	void PDDLPrint( std::ostream & s ) const {
	}

};

#endif
