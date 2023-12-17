#ifndef EXPR_H
#define EXPR_H

#include <iostream>
#include <sstream>
#include <string>
using namespace std;
class Expression {
 public:
  virtual string toString() const = 0;
  virtual long evaluate() const = 0;
  virtual ~Expression() {}
};

class BinaryExpression : public Expression {
 protected:
  Expression * left;
  Expression * right;
  BinaryExpression(Expression * lhs, Expression * rhs) : left(lhs), right(rhs) {}
  virtual string toStringWithOperator(char c) const {
    stringstream ss("");
    ss << "(";
    ss << left->toString();
    ss << " ";
    ss << c;
    ss << " ";
    ss << right->toString();
    ss << ")";
    return ss.str();
  }

 public:
  virtual ~BinaryExpression() {
    delete left;
    delete right;
  }
};

class NumExpression : public Expression {
 private:
  long val;

 public:
  NumExpression(long value) : val(value) {}
  virtual ~NumExpression() {}
  virtual string toString() const {
    stringstream ss("");
    ss << val;
    return ss.str();
  }
  virtual long evaluate() const { return val; };
};
class PlusExpression : public BinaryExpression {
 public:
  PlusExpression(Expression * lhs, Expression * rhs) : BinaryExpression(lhs, rhs) {}
  virtual ~PlusExpression() {}
  virtual string toString() const { return toStringWithOperator('+'); }
  virtual long evaluate() const { return left->evaluate() + right->evaluate(); }
};

class MinusExpression : public BinaryExpression {
 public:
  MinusExpression(Expression * lhs, Expression * rhs) : BinaryExpression(lhs, rhs) {}
  virtual ~MinusExpression() {}
  virtual string toString() const { return toStringWithOperator('-'); }
  virtual long evaluate() const { return left->evaluate() - right->evaluate(); }
};
class TimesExpression : public BinaryExpression {
 public:
  TimesExpression(Expression * lhs, Expression * rhs) : BinaryExpression(lhs, rhs) {}
  virtual ~TimesExpression() {}
  virtual string toString() const { return toStringWithOperator('*'); }
  virtual long evaluate() const { return left->evaluate() * right->evaluate(); }
};
class DivExpression : public BinaryExpression {
 public:
  DivExpression(Expression * lhs, Expression * rhs) : BinaryExpression(lhs, rhs) {}
  virtual ~DivExpression() {}
  virtual string toString() const { return toStringWithOperator('/'); }
  virtual long evaluate() const { return left->evaluate() / right->evaluate(); }
};
#endif
