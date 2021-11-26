add_test( Utility.const_pow /home/andreas/git/logger/cmake-build-release/test/utility_test [==[--gtest_filter=Utility.const_pow]==] --gtest_also_run_disabled_tests)
set_tests_properties( Utility.const_pow PROPERTIES WORKING_DIRECTORY /home/andreas/git/logger/cmake-build-release/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] VS_DEBUGGER_WORKING_DIRECTORY)
set( utility_test_TESTS Utility.const_pow)
