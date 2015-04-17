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
		// replace with path to the IPC2 Logistics (typed) domain
		std::string path = "/home/anders/projects/inference/IPC/IPC2/Logistics/Track1/Typed/";

		Domain dom( path + "domain.pddl" );
		Instance ins( dom, path + "probLOGISTICS-4-0.pddl" );

		checkEqual( dom, "tests/Log_dom.pddl" );
		checkEqual( ins, "tests/Log_ins.pddl" );
	}

	void scheduleTest() {
		// replace with path to the IPC2 Schedule (STRIPS) domain
		std::string path = "/home/anders/projects/inference/IPC/IPC2/Schedule/Strips/";

		Domain dom( path + "domain.pddl" );
		Instance ins( dom, path + "probschedule-2-0.pddl" );

		checkEqual( dom, "tests/Sched_dom.pddl" );
		checkEqual( ins, "tests/Sched_ins.pddl" );
	}

	void temporalTest() {
		// replace with path to the IPC-2014 Mapanalyser temporal domain
		std::string path = "/home/anders/ownCloud/research/temporal/compilingtemporal/domains/tempo-sat/Mapanalyser/";

		Domain dom( path + "domain/domain.pddl" );
		Instance ins( dom, path + "problems/pfile3-4-2-0-1.pddl" );

		checkEqual( dom, "tests/Mapana_dom.pddl" );
		checkEqual( ins, "tests/Mapana_ins.pddl" );
	}

	void multiagentTest() {
		// replace with path to the CoDMAP-2015 Logistics multiagent domain
		std::string path = "/home/anders/ownCloud/research/multiagent/codmap/domains/logistics00/";

		Domain dom( path + "domain.pddl" );
		Instance ins( dom, path + "probLOGISTICS-4-0.pddl" );

		checkEqual( dom, "tests/Multilog_dom.pddl" );
		checkEqual( ins, "tests/Multilog_ins.pddl" );
	}
};

REGISTER_FIXTURE( DomainTests )

