MACRO (add_gtest TESTNAME)
    target_link_libraries (
            ${TESTNAME}
            PRIVATE gtest
            PRIVATE gmock
            PRIVATE gtest_main)
    add_test (${TESTNAME} ${TESTNAME})
    set_target_properties (${TESTNAME} PROPERTIES FOLDER "Tests")
ENDMACRO ()