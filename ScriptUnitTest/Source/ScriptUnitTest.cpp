#include <gtest\gtest.h>

int main(int argc, char* argv[])
{
	::testing::GTEST_FLAG(output) = "xml";
	::testing::InitGoogleTest(&argc, argv);
	//::testing::GTEST_FLAG(filter) = "ScriptEngine.*";
	RUN_ALL_TESTS();

	return 0;
}