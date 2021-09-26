#include "gtest/gtest.h"
#include "ExampleLogic/MathOperator.h"

namespace
{
TEST(MathOperatorTest, AddInts) {
  const int a = 5;
  const int b = 10;
  MathOperator<int> op (a, b);
  ASSERT_EQ(op.Add(), 15);
}
TEST(MathOperatorTest, SubtractFloats) {
  const float a = 5.f;
  const float b = 3.f;
  MathOperator<float> op (a, b);
  ASSERT_EQ(op.Subtract(), 2.f);
}
TEST(MathOperatorTest, MultiplyDoubles) {
  const double a = 2.5;
  const double b = 3.;
  MathOperator<double> op (a, b);
  ASSERT_EQ(op.Multiply(), 7.5);
}
}