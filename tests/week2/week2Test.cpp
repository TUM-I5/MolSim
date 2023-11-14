#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Create a mock class
class MockFoo {
public:
    MOCK_METHOD(int, getValue, (), (const));
};

// Create a test fixture
class MockTest : public testing::Test {
protected:
    MockFoo foo;
};

// Test using the mock object
TEST_F(MockTest, TestMockFoo) {
    // Set an expectation on the mock object
    EXPECT_CALL(foo, getValue())
        .WillOnce(testing::Return(42));

    // Use the mock object
    int value = foo.getValue();
    EXPECT_EQ(value, 42);
}