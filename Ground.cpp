
#include "Domain.h"

Ground::Ground( const Ground * g, Domain & d )
	: ParamCond( g ), lifted( d.preds.get( g->name ) ) {}

void Ground::PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, Domain & d ) {
	tabindent( s, indent );
	s << "( " << name;
	for ( unsigned i = 0; i < params.size(); ++i ) {
		if ( ts.size() && params[i] >= 0 ) s << " " << ts[params[i]];
		else s << " " << d.types[lifted->params[i]]->object( params[i] ).first;
	}
	s << " )";
}

void Ground::parse( Filereader & f, TokenStruct< std::string > & ts, Domain & d ) {
	f.next();
	params.resize( lifted->params.size() );
	for ( unsigned i = 0; i < lifted->params.size(); ++i, f.next() ) {
		std::string s = f.getToken();
		int k = ts.index( s );
		if ( k >= 0 ) params[i] = k;
		else {
			std::pair< bool, int > p = d.types[lifted->params[i]]->parseConstant( s );
			if ( p.first ) params[i] = p.second;
			else f.tokenExit( s );
		}
	}
	f.assert( ")" );
}
