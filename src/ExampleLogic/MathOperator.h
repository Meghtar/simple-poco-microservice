#pragma once

template<class T>
class MathOperator
{
public:
  MathOperator(const T val1, const T val2)
    : _val1(val1)
    , _val2(val2)
  {}
  T Add() { return _val1 + _val2; }
  T Subtract() { return _val1 - _val2; }
  T Multiply() { return _val1 * _val2; }

private:
  const T _val1;
  const T _val2;
};