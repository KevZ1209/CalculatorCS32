//
//  main.cpp
//  Eval
//
//  Created by Kevin Zhang on 1/28/22.
//

#include <iostream>
#include <cctype>
#include <stack>
#include <string>
#include <cassert>
#include "Map.h"
using namespace std;

bool is_operator(const char ch) {
    if (ch == '/' || ch == '+' || ch == '*' || ch == '-') {
        return true;
    }
    return false;
}

int do_math(int a, int b, char op) {
    if (op == '+') {
        return a + b;
    }
    else if (op == '-') {
        return a - b;
    }
    else if (op == '/') {
        return a / b;
    }
    else if (op == '*') {
        return a * b;
    }
    else {
        return 999;
    }
}

bool check_precedence(const char a, const char b) {
    // return true if precedence of a is <= precedence of b
    if (a == '/' || a == '*') {
        if (b == '/' || b == '*') {
            return true;
        }
        return false;
    }
    return true;
}

int check_expression(const string infix) {
    
    
    return 1;
}

int evaluate(string infix, const Map& values, string& postfix, int& result) {
    // Evaluates an integer arithmetic expression
              //   If infix is a syntactically valid infix integer expression whose
              //   only operands are single lower case letters (whether or not they
              //   appear in the values map), then postfix is set to the postfix
              //   form of the expression; otherwise postfix may or may not be
              //   changed, result is unchanged, and the function returns 1.  If
              //   infix is syntactically valid but contains at least one lower
              //   case letter operand that does not appear in the values map, then
              //   result is unchanged and the function returns 2.  If infix is
              //   syntactically valid and all its lower case operand letters
              //   appear in the values map, then if evaluating the expression
              //   (using for each letter in the expression the value in the map
              //   that corresponds to it) attempts to divide by zero, then result
              //   is unchanged and the function returns 3; otherwise, result is
              //   set to the value of the expression and the function returns 0.
    
    // Infix to Postfix Conversion
    stack<char> operator_stack;
    postfix = "";
    
    for (int i = 0; i < infix.size(); i++) {
        char ch = infix[i];
        
        if (isalpha(ch) && islower(ch))
            postfix += ch;
        
        if (ch == '(')
            operator_stack.push(ch);
        
        if (ch == ')') {
            while (operator_stack.top() != '(') {
                postfix += operator_stack.top();
                operator_stack.pop();
            }
            operator_stack.pop();
        }
        
        if (is_operator(ch)) {
            while (!operator_stack.empty() && operator_stack.top() != '(' && check_precedence(ch, operator_stack.top())) {
                postfix += operator_stack.top();
                operator_stack.pop();
            }
            operator_stack.push(ch);
        }
    }
    
    while (!operator_stack.empty()) {
        postfix += operator_stack.top();
        operator_stack.pop();
    }
    
    cerr << postfix << endl;
    
    // evaluating postfix expression
    stack<int> operand_stack;
    for (int i = 0; i < postfix.size(); i++) {
        char ch = postfix[i];
        if (isalpha(ch) && islower(ch)) {
            int value;
            values.get(ch, value);
            operand_stack.push(value);
        }
        else // ch is binary operator
        {
            char operand2 = operand_stack.top();
            operand_stack.pop();
            char operand1 = operand_stack.top();
            operand_stack.pop();
            operand_stack.push(do_math(operand1, operand2, ch));
        }
    }
    
    result = operand_stack.top();
    
    return true;
}



int main() {
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    
    string pf;
    int answer;
    
    evaluate("a+i*o+y", m, pf, answer);
    
    cerr << answer << endl;
    
}
