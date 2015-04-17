
#ifndef _FORALL_H_
#define _FORALL_H_

#include "ParamCond.h"

class Forall : public ParamCond {

public:

	Condition * cond;

	Forall()
		: cond( 0 ) {}

	Forall( const Forall * f, Domain & d )
		: ParamCond( f ), cond( 0 ) {
		if ( f->cond ) cond = f->cond->copy( d );
	}

	~Forall() {
		if ( cond ) delete cond;
	}

	void print( std::ostream & s ) const {
		s << "Forall" << params << ":\n";
		cond->print( s );
	}

	void PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, Domain & d );

	void parse( Filereader & f, TokenStruct< std::string > & ts, Domain & d );

	void addParams( int m, unsigned n ) {
		cond->addParams( m, n );
	}

	Condition * copy( Domain & d ) {
		return new Forall( this, d );
	}

};

#endif
