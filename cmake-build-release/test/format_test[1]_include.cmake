if(EXISTS "/home/andreas/git/logger/cmake-build-release/test/format_test[1]_tests.cmake")
  include("/home/andreas/git/logger/cmake-build-release/test/format_test[1]_tests.cmake")
else()
  add_test(format_test_NOT_BUILT format_test_NOT_BUILT)
endif()