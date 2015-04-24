#include <MiniCppUnit.h>
#include "Instance.h"

class DomainTests : public TestFixture< DomainTests >
{
public:
	TEST_FIXTURE( DomainTests )
	{
		TEST_CASE( logisticsTest );
		TEST_CASE( scheduleTest );
		TEST_CASE( temporalTest );
		TEST_CASE( multiagentTest );
	}

	template < typename T >
	void checkEqual( T & prob, const std::string & file ) {
		std::ifstream f( file.c_str() );
		std::string s, t;
		for ( std::getline( f, s ); !f.eof(); std::getline( f, s ) )
			t += s + "\n";

		std::stringstream ds;
		prob.PDDLPrint( ds );
		ASSERT_EQUALS( t, ds.str() );
	}

	void logisticsTest() {
		Domain dom( "domains/Log_dom.pddl" );
		Instance ins( dom, "domains/Log_ins.pddl" );

		checkEqual( dom, "tests/Log_dom.pddl" );
		checkEqual( ins, "tests/Log_ins.pddl" );
	}

	void scheduleTest() {
		Domain dom( "domains/Sched_dom.pddl" );
		Instance ins( dom, "domains/Sched_ins.pddl" );

		checkEqual( dom, "tests/Sched_dom.pddl" );
		checkEqual( ins, "tests/Sched_ins.pddl" );
	}

	void temporalTest() {
		Domain dom( "domains/Mapana_dom.pddl" );
		Instance ins( dom, "domains/Mapana_ins.pddl" );

		checkEqual( dom, "tests/Mapana_dom.pddl" );
		checkEqual( ins, "tests/Mapana_ins.pddl" );
	}

	void multiagentTest() {
		Domain dom( "domains/Multilog_dom.pddl" );
		Instance ins( dom, "domains/Multilog_ins.pddl" );

		checkEqual( dom, "tests/Multilog_dom.pddl" );
		checkEqual( ins, "tests/Multilog_ins.pddl" );
	}
};

REGISTER_FIXTURE( DomainTests )

