
#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "Domain.h"

class Instance {
public:
	Domain &d;
	std::string name;
	TypeGroundVec init, goal; // initial and goal states
	bool metric;

	Instance( Domain & dom )
		: d( dom ), metric( false ) {}

	Instance( Domain & dom, const std::string & s )
		: d( dom ), metric( false ) {
		parse(s);
	}

	~Instance() {
		for ( unsigned i = 0; i < init.size(); ++i )
			delete init[i];
		for ( unsigned i = 0; i < goal.size(); ++i )
			delete goal[i];
	}

	void parse( const std::string &s) {
		Filereader f( s );
		name = f.parseName( "PROBLEM" );

		if ( DOMAIN_DEBUG ) std::cout << name << "\n";
    
		for ( ; f.getChar() != ')'; f.next() ) {
			f.assert( "(" );
			f.assert( ":" );
			std::string t = f.getToken();

			if ( DOMAIN_DEBUG ) std::cout << t << "\n";

			if ( t == "DOMAIN" ) parseDomain( f );
			else if ( t == "OBJECTS" ) parseObjects( f );
			else if ( t == "INIT" ) parseInit( f );
			else if ( t == "GOAL" ) parseGoal( f );
			else if ( t == "METRIC" ) parseMetric( f );
			else f.tokenExit( t );
		}
	}

	void parseDomain( Filereader & f ) {
		f.next();
		f.assert( d.name );
		f.assert( ")" );
	}

	void parseObjects( Filereader & f ) {
		TokenStruct< std::string > ts = f.parseTypedList( true, d.types );

		for ( unsigned i = 0; i < ts.size(); ++i )
			d.getType( ts.types[i] )->objects.insert( ts[i] );

		for ( unsigned i = 0; DOMAIN_DEBUG && i < d.types.size(); ++i ) {
			std::cout << " ";
			if ( d.typed ) std::cout << " " << d.types[i] << ":";
			for ( unsigned j = 0; j < d.types[i]->objects.size(); ++j )
				std::cout << " " << d.types[i]->objects[j];
			std::cout << "\n";
		}
	}

	void parseGround( Filereader & f, TypeGroundVec & v ) {
		TypeGround * c = 0;
		if ( f.getChar() == '=' ) {
			f.assert( "=" );
			f.assert( "(" );

			std::string s = f.getToken();
			int i = d.funcs.index( s );
			if ( i < 0 ) f.tokenExit( s );
			
			if ( d.funcs[i]->returnType < 0 ) c = new GroundFunc< double >( d.funcs[i] );
			else c = new GroundFunc< int >( d.funcs[i] );
		}
		else c = new TypeGround( d.preds.get( f.getToken( d.preds ) ) );
		
		c->parse( f, d.types[0]->constants, d );
		v.push_back( c );
	}

	void parseInit( Filereader & f ) {
		for ( f.next(); f.getChar() != ')'; f.next() ) {
			f.assert( "(" );
			parseGround( f, init );
		}
		++f.c;

		for ( unsigned i = 0; DOMAIN_DEBUG && i < init.size(); ++i )
			std::cout << "  " << init[i];
	}

	void parseGoal( Filereader & f ) {
		f.next();
		f.assert( "(" );
		std::string s = f.getToken();
		if ( s == "AND" ) {
			for ( f.next(); f.getChar() != ')'; f.next() ) {
				f.assert( "(" );
				parseGround( f, goal );
			}
			++f.c;
			f.next();
		}
		else {
			f.c -= s.size();
			parseGround( f, goal );
		}
		f.assert( ")" );

		for ( unsigned i = 0; DOMAIN_DEBUG && i < goal.size(); ++i )
			std::cout << "  " << goal[i];
	}

	// for the moment only parse total-time
	void parseMetric( Filereader & f ) {
		metric = true;

		f.next();
		f.assert( "MINIMIZE" );
		f.assert( "(" );
		f.assert( "TOTAL-TIME" );
		f.assert( ")" );
		f.assert( ")" );
	}

	// add an object of a certain type
	void addObject( const std::string & name, const std::string & type ) {
		d.getType( type )->objects.insert( name );
	}

	// add a predicate fluent to the initial state
	void addInit( const std::string & name, const StringVec & v = StringVec() ) {
		TypeGround * tg = new TypeGround( d.preds.get( name ) );
		tg->insert( d, v );
		init.push_back( tg );
	}

	// add a function value to the initial state
	void addInit( const std::string & name, int value, const StringVec & v = StringVec() ) {
		GroundFunc< int > * gf = new GroundFunc< int >( d.funcs.get( name ), value );
		gf->insert( d, v );
		init.push_back( gf );
	}

	// add a function value to the initial state
	void addInit( const std::string & name, double value, const StringVec & v = StringVec() ) {
		GroundFunc< double > * gf = new GroundFunc< double >( d.funcs.get( name ), value );
		gf->insert( d, v );
		init.push_back( gf );
	}

	// add a fluent (predicate or function) to the initial state
	void addInit( TypeGround * g, const StringVec & v = StringVec() ) {
		TypeGround * tg = 0;
		GroundFunc< int > * f1 = dynamic_cast< GroundFunc< int > * >( g );
		GroundFunc< double > * f2 = dynamic_cast< GroundFunc< double > * >( g );
		if ( f1 ) tg = new GroundFunc< int >( d.funcs.get( g->name ), f1->value );
		else if ( f2 ) tg = new GroundFunc< double >( d.funcs.get( g->name ), f2->value );
		else tg = new TypeGround( d.preds.get( g->name ) );
		tg->insert( d, v );
		init.push_back( tg );
	}

	// add a fluent to the goal state
	void addGoal( const std::string & name, const StringVec & v = StringVec() ) {
		TypeGround * tg = new TypeGround( d.preds.get( name ) );
		tg->insert( d, v );
		goal.push_back( tg );
	}

	void PDDLPrint( std::ostream & stream ) {
		stream << "( DEFINE ( PROBLEM " << name << " )\n";
		stream << "( :DOMAIN " << d.name << " )\n";

		stream << "( :OBJECTS\n";
		for ( unsigned i = 0; i < d.types.size(); ++i )
			if ( d.types[i]->objects.size() ) {
				stream << "\t";
				for ( unsigned j = 0; j < d.types[i]->objects.size(); ++j )
					stream << d.types[i]->objects[j] << " ";
				if ( d.typed ) stream << "- " << d.types[i]->name;
				stream << "\n";
			}
		stream << ")\n";

		stream << "( :INIT\n";
		for ( unsigned i = 0; i < init.size(); ++i ) {
			init[i]->PDDLPrint( stream, 1, TokenStruct< std::string >(), d );
			stream << "\n";
		}
		stream << ")\n";

		stream << "( :GOAL\n";
		stream << "\t( AND\n";
		for ( unsigned i = 0; i < goal.size(); ++i ) {
			goal[i]->PDDLPrint( stream, 2, TokenStruct< std::string >(), d );
			stream << "\n";
		}
		stream << "\t)\n";
		stream << ")\n";

		if ( metric )
			stream << "( :METRIC MINIMIZE ( TOTAL-TIME ) )\n";

		stream << ")\n";
	}

};

#endif
