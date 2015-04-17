
#include "Domain.h"

Condition * createCondition( Filereader & f, Domain & d ) {
	std::string s = f.getToken();

	if ( s == "AND" ) return new And;
	if ( s == "FORALL" ) return new Forall;
	if ( s == "NOT" ) return new Not;
	if ( s == "OR" ) return new Or;
	if ( s == "WHEN" ) return new When;

	int i = d.preds.index( s );
	if ( i >= 0 ) return new Ground( d.preds[i] );

	f.tokenExit( s );

	return 0;
}

