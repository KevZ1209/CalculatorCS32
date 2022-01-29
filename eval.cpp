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

bool is_paren(const char ch) {
    if (ch == '(' || ch == ')') {
        return true;
    }
    return false;
}

void remove_parentheses(string& s) {
    string result = "";
    for (int i = 0; i < s.size(); i++) {
        if (!is_paren(s[i])) {
            result+=s[i];        }
    }
    s = result;
}

bool is_operator(const char ch) {
    if (ch == '/' || ch == '+' || ch == '*' || ch == '-') {
        return true;
    }
    return false;
}

bool is_operand(const char c) {
    return (isalpha(c) && islower(c));
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

void remove_spaces(string& infix) {
    int i = 0;
    string result = "";
    while (i < infix.size()) {
        if (infix[i] != ' ') {
            result += infix[i];
        }
        i++;
    }
    infix = result;
}

bool is_valid_expression(string& infix) {
    remove_spaces(infix);
    
    if (infix.size() == 0) {
        return false;
    }
    
    else if (infix.size() == 1) {
        return is_operand(infix[0]);
    }
    
    // make sure every character is valid
    for (int i = 0; i < infix.size(); i++) {
        if (!is_operand(infix[i]) && !is_operator(infix[i]) && !is_paren(infix[i])) {
            cerr << "case 1" << endl;
            return false;
        }
    }
    
    // # of ('s should equal # of )'s
    int count_left_paren = 0;
    int count_right_paren = 0;
    
    for (int i = 0; i < infix.size(); i++) {
        if (infix[i] == '(') {
            count_left_paren++;
        }
        else if (infix[i] == ')') {
            count_right_paren++;
        }
    }
    if (count_left_paren != count_right_paren) {
        // syntactically invalid
        cerr << "case 2" << endl;
        return false;
    }
    
    // check if theres a '()'
    for (int i = 0; i < infix.size()-1; i++) {
        if (infix[i] == '(' && infix[i+1] == ')') {
            return false;
        }
    }
    
    // check if there's an operator right after '(' or right before ')'
    for (int i = 1; i < infix.size()-1; i++) {
        if (infix[i] == '(' && is_operator(infix[i+1])) {
            cerr << "case 6" << endl;
            return false;
        }
        if (infix[i] == ')' && is_operator(infix[i-1])) {
            cerr << "case 7" << endl;
            return false;
        }
    }
    
    // From now on, ignore parentheses. Pattern should be operand, operator, operand, operator, operand...
    
    string ignore_paren = infix;
    remove_parentheses(ignore_paren);
    
    for (int i = 0; i < ignore_paren.size(); i+=2) {
        if (!is_operand(ignore_paren[i])) {
            cerr << "case 3" << endl;
            return false;
        }
    }
    for (int i = 1; i < ignore_paren.size(); i+=2) {
        if (!is_operator(ignore_paren[i])) {
            cerr << "case 4" << endl;
            return false;
        }
    }
    
    // make sure last character isn't an operator
    if (is_operator(ignore_paren[ignore_paren.size()-1])) {
        cerr << "case 5" << endl;
        return false;
    }
    
    // infix is syntactically invalid
    
    
    // valid expression
    return true;
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
    
    remove_spaces(infix);
    
    // Infix to Postfix Conversion
    stack<char> operator_stack;
    postfix = "";
    
    for (int i = 0; i < infix.size(); i++) {
        char ch = infix[i];
        
        if (is_operand(ch))
            postfix += ch;
        
        else if (ch == '(')
            operator_stack.push(ch);
        
        else if (ch == ')') {
            while (operator_stack.top() != '(') {
                postfix += operator_stack.top();
                operator_stack.pop();
            }
            operator_stack.pop();
        }
        
        else if (is_operator(ch)) {
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
    
    if (!is_valid_expression(infix)) {
        return 1;
    }
    
    // evaluating postfix expression
    stack<int> operand_stack;
    for (int i = 0; i < postfix.size(); i++) {
        char ch = postfix[i];
        if (is_operand(ch)) {
            int value;
            if (values.get(ch, value) == false) {
                // if ch key doesn't exist in map
                cerr << "CHARACTER " << ch << " DOESN'T EXIST IN MAP" << endl;
                return 2;
            }
            operand_stack.push(value);
        }
        else // ch is binary operator
        {
            char operand2 = operand_stack.top();
            operand_stack.pop();
            char operand1 = operand_stack.top();
            operand_stack.pop();
            // check division by zero
            if (ch == '/' || operand2 == 0) {
                cerr << "DIVISION BY ZERO" << endl;
                return 3;
            }
            operand_stack.push(do_math(operand1, operand2, ch));
        }
    }
    
    result = operand_stack.top();
    
    return 0;
}



int main()
        {
            char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
            int  vals[] = {  3,  -9,   6,   2,   4,   1  };
            Map m;
            for (int k = 0; vars[k] != '#'; k++)
                m.insert(vars[k], vals[k]);
            string pf;
            int answer;
            assert(evaluate("a+ e", m, pf, answer) == 0  &&
                                    pf == "ae+"  &&  answer == -6);
            answer = 999;
            assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("()o", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("y(*o)", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
              // unary operators not allowed:
            assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("a*b", m, pf, answer) == 2  &&
                                    pf == "ab*"  &&  answer == 999);
            assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
                                    pf == "yoau-*+"  &&  answer == -1);
            answer = 999;
            assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
                                    pf == "oyy-/"  &&  answer == 999);
            assert(evaluate(" a  ", m, pf, answer) == 0  &&
                                    pf == "a"  &&  answer == 3);
            assert(evaluate("((a))", m, pf, answer) == 0  &&
                                    pf == "a"  &&  answer == 3);
            cout << "Passed all tests" << endl;
        }
