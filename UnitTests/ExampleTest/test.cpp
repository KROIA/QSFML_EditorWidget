#include "test.h"
#include <iostream>
#include <windows.h>

#define color_black      0
#define color_dark_blue  1
#define color_dark_green 2
#define color_light_blue 3
#define color_dark_red   4
#define color_magenta    5
#define color_orange     6
#define color_light_gray 7
#define color_gray       8
#define color_blue       9
#define color_green     10
#define color_cyan      11
#define color_red       12
#define color_pink      13
#define color_yellow    14
#define color_white     15



Test::Test(const std::string& name)
	: m_name(name)
	, m_breakTestOnFail(true)
{
	std::vector<Test*>& s_tests = getTestsInternal();
	s_tests.push_back(this);
}
Test::~Test()
{
	std::vector<Test*>& s_tests = getTestsInternal();
	auto &it = std::find(s_tests.begin(), s_tests.end(), this);
	if (it != s_tests.end())
		s_tests.erase(it);
}


bool Test::runTests()
{
	m_results = TestResults();
	bool success = runTests(m_results);
	m_results.success = success;
	return success;
}
bool Test::runTests(TestResults& results)
{
	results.name = m_name;
	results.subResults.reserve(m_testFunctions.size());

	bool success = true;
	for (size_t i = 0; i < m_testFunctions.size(); ++i)
	{
		TestResults r;
		success &= m_testFunctions[i](r);
		results.subResults.push_back(r);
	}
	for (size_t i = 0; i < m_subTests.size(); ++i)
	{
		TestResults r;
		success &= m_subTests[i]->runTests(r);
		results.subResults.push_back(r);
	}
	results.success = success;
	m_results = results;
	return success;
}

const Test::TestResults& Test::getResults() const
{
	return m_results;
}

void Test::setBreakOnFail(bool breakOnFail)
{
	m_breakTestOnFail = breakOnFail;
}
bool Test::doesBreakOnFail() const
{
	return m_breakTestOnFail;
}

void Test::printResults() const
{
	printResultsRecursive(m_results, 0);
}
void Test::printResultsRecursive(const TestResults& results, int depth)
{
	for (int i = 0; i < depth; ++i)
		std::cout << " | ";
	int color = color_white;
	std::cout << " +-"<<results.name << ": ";
	if (results.success)
		printColored("PASS", color_green);
	else
		printColored("FAIL", color_red);
	std::cout << std::endl;

	for (size_t i = 0; i < results.subResults.size(); ++i)
	{
		printResultsRecursive(results.subResults[i], depth + 1);
	}
	

	


	for (size_t i = 0; i < results.results.size(); ++i)
	{
		for (int i = 0; i < depth; ++i)
			std::cout << " | ";
		std::string stateString;

		// print the state in the correct color
		color = color_white;
		switch (results.results[i].state)
		{
		case ResultState::pass:
			stateString = "PASS";
			color = color_green;
			break;
		case ResultState::fail:
			stateString = "FAIL";
			color = color_red;
			break;
		case ResultState::none:
			stateString = "NONE";
			break;
		}
		std::cout << " |   "<< results.results[i].message << ": ";
		printColored(stateString, color);
		std::cout << std::endl;
	}
	for (int i = 0; i < depth; ++i)
		std::cout << " | ";

	std::cout << " | \"" << results.name << "\" Testresult: ";
	if (results.success)
		printColored("PASS", color_green);
	else
		printColored("FAIL", color_red);

	std::cout << std::endl;
	for (int i = 0; i < depth; ++i)
		std::cout << " | ";
	std::cout <<" +--------------------------------------\n";
	for (int i = 0; i < depth; ++i)
		std::cout << " | ";
	std::cout << std::endl;
	
}
void Test::printColored(const std::string& str, int color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	// Read old color
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	WORD oldColor = consoleInfo.wAttributes;


	// pick the colorattribute k you want
	SetConsoleTextAttribute(hConsole, color);
	std::cout << str;
	SetConsoleTextAttribute(hConsole, oldColor);
}

const std::vector<Test*>& Test::getTests()
{
	return getTestsInternal();
}
std::vector<Test*>& Test::getTestsInternal()
{
	static std::vector<Test*> tests;
	return tests;
}
bool Test::runAllTests(TestResults& results)
{
	results.name = "All Tests";
	bool success = true;
	const std::vector<Test*>& s_tests = getTests();
	for (size_t i = 0; i < s_tests.size(); ++i)
	{
		TestResults r;
		success &= s_tests[i]->runTests(r); 
		results.subResults.emplace_back(std::move(r));
	}
	results.success = success;
	return success;
}
void Test::printResults(const TestResults& results)
{
	printResultsRecursive(results, 0);
}

/*void Test::addTest(TestFunction func)
{
	m_testFunctions.push_back(func);
}*/
void Test::addTest(Test* subTest)
{
	m_subTests.push_back(subTest);
}
