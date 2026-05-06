//! This file implements testing for gbox's result type

#include "gbox/core/result.hpp"

#include <gtest/gtest.h>

using namespace gbox;
using gbox::result::Err;
using gbox::result::Ok;

/// Test error kind
enum class ErrorKind {
    /// Test failure variant
    Any
};

/// Test result alias
template <typename T>
using Result = result::Result<T, ErrorKind>;

/// Test to see if we can explicitly build a result of
/// a given type using the Ok builder
TEST(resultOkBuilder, resultTests) { Result<int> res = Ok(1); }

/// Test to see if we can explicitly build a result of
/// a given type using the Err builder
TEST(resultErrBuilder, resultTests) { Result<int> res = Err(ErrorKind::Any); }

/// Test to see if we can explicitly build a result of
/// a void type using the Err builder
TEST(resultOkVoidBuilder, resultTests) { Result<void> res = Ok(); }

/// Test to see if we can unwrap an Ok value result with no throw
TEST(resultUnwrapOk, resultTests) {
    Result<int> res = Ok(1);
    ASSERT_EQ(1, res.unwrap());
}

/// Test to see if we can unwrap a void type value result with
/// no throw
TEST(resultUnwrapOkVoid, resultTests) {
    Result<void> res = Ok();
    res.unwrap();
}

/// Test to see if we can unwrap an error type result with no throw
TEST(resultUnwrapErr, resultTests) {
    Result<void> res = Err(ErrorKind::Any);
    res.unwrap_err();
}

// TODO: Fill out the result api...
