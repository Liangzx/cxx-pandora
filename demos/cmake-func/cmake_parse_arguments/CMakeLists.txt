cmake_minimum_required(VERSION 3.5)

project(test)
# 参考 absl/CMake/AbseilHelpers.cmake.absl_cc_library
function(test_parse)
    set( options op1 op2 op3 )
    set( oneValueArgs v1 v2 v3 )
    set( multiValueArgs m1 m2 )
    message( STATUS "options = ${options}" )
    message( STATUS "oneValueArgs = ${oneValueArgs}" )
    message( STATUS "multiValueArgs = ${multiValueArgs}" )
    cmake_parse_arguments( MYPRE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    message("op1  = ${MYPRE_op1}")
    message("op2  = ${MYPRE_op2}")
    message("op3  = ${MYPRE_op3}")

    message("v1  = ${MYPRE_v1}")
    message("v2  = ${MYPRE_v2}")
    message("v3  = ${MYPRE_v3}")

    message("m1  = ${MYPRE_m1}")
    message("m2  = ${MYPRE_m2}")
endfunction()

message( STATUS "\n" )
test_parse( op1 op2 op3 v1 aaa v2 111 v3 bbb m1 1 2 3 4 5 m2 a b c )

message( STATUS "\n" )
test_parse(op1 op3 v1 aaa v2 111 v3 bbb m1 1 2 3 4 5 m2 a b c )

message( STATUS "\n" )
test_parse(op1 op3 v1 aaa v2 v3 bbb m1 1 2 3 4 5  )

message( STATUS "\n" )
test_parse( op1  v1 aaa v2 op2 111 v3 bbb m1 1 2 3 4 5 m2 a b c op3 )