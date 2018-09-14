//
//  main.cpp
//  Calculator
//
//  Created by Ethan Xu on 31/7/17.
//  Copyright © 2017 Ethan Xu. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stdexcept>

//a class for parsing token
class Number {
    
private:
    int i;
    double d;
    bool int_or_double;
    
    bool isInteger(const std::string &s) {
        return !s.empty()
        && std::find_if(s.begin(), s.end(), [](char c) {return !std::isdigit(c);}) == s.end();
    }
    
public:
    Number(const std::string &s) {
        if (isInteger(s)) {
            i = std::stoi(s);
            d = 0;
            int_or_double = true;
        } else {
            i = 0;
            d = std::stod(s);
            int_or_double = false;
        }
    }
    
    Number(int i) {
        this->i = i;
        int_or_double = true;
    }
    
    Number(double d) {
        this->d = d;
        int_or_double = false;
    }
    
    bool is_int_or_double () {
        return int_or_double;
    }
    
    int getIntegerValue() {
        return int_or_double ? i : static_cast<int>(d);
    }
    
    double getDoubleValue() {
        return int_or_double ? i : d;
    }
};

//override << operator
std::ostream &operator<<(std::ostream &os, Number &number) {
    if (number.is_int_or_double()) {
        return os << number.getIntegerValue();
    } else {
        return os << number.getDoubleValue();
    }
}

std::stack<std::string> stack;

//handle a token vector recursively
void handleToken(const std::vector<std::string>& vector) {
    std::vector<std::string> repeatVector;//store all the tokens between 'repeat' and 'endrepeat'
    int repeat = 0;//repeat time
    for (auto const& s: vector) {
        if (repeat != 0) {//storing the repeating tokens
            if (s.compare("endrepeat") == 0) {
                std::vector<std::string> toHandleRepeatTokenVector;
                for (int i = 0; i < repeat; i++) {
                    for (auto const& s: repeatVector) {
                        toHandleRepeatTokenVector.push_back(s);
                    }
                }
                handleToken(toHandleRepeatTokenVector);
                toHandleRepeatTokenVector.clear();
                repeatVector.clear();
                repeat = 0;
            } else {
                repeatVector.push_back(s);
            }
        } else {
            if (s.compare("add") == 0
                || s.compare("sub") == 0
                || s.compare("mult") == 0
                || s.compare("div") == 0) {
                std::string as = stack.top();
                stack.pop();
                std::string bs = stack.top();
                stack.pop();
                
                Number an (as);
                Number bn (bs);
                std::string op;
                Number *cn = NULL;
                
                if (s.compare("add") == 0) {
                    if (an.is_int_or_double() && bn.is_int_or_double()) {
                        cn = new Number(an.getIntegerValue() + bn.getIntegerValue());
                    } else {
                        cn = new Number(an.getDoubleValue() + bn.getDoubleValue());
                    }
                    op = " + ";
                } else if (s.compare("sub") == 0) {
                    if (an.is_int_or_double() && bn.is_int_or_double()) {
                        cn = new Number(an.getIntegerValue() - bn.getIntegerValue());
                    } else {
                        cn = new Number(an.getDoubleValue() - bn.getDoubleValue());
                    }
                    op = " - ";
                } else if (s.compare("mult") == 0) {
                    if (an.is_int_or_double() && bn.is_int_or_double()) {
                        cn = new Number(an.getIntegerValue() * bn.getIntegerValue());
                    } else {
                        cn = new Number(an.getDoubleValue() * bn.getDoubleValue());
                    }
                    op = " * ";
                } else if (s.compare("div") == 0) {
                    if (an.is_int_or_double() && bn.is_int_or_double()) {
                        cn = new Number(an.getIntegerValue() / bn.getIntegerValue());
                    } else {
                        cn = new Number(an.getDoubleValue() / bn.getDoubleValue());
                    }
                    op = " / ";
                }
                
                std::cout << an << op << bn << " = " << *cn << std::endl;
                stack.push((*cn).is_int_or_double() ?
                           std::to_string((*cn).getIntegerValue()) :
                           std::to_string((*cn).getDoubleValue()));
            } else if (s.compare("sqrt") == 0) {
                std::string as = stack.top();
                stack.pop();
                
                Number an(as);
                Number bn(std::sqrt(an.getDoubleValue()));
                
                std::cout << "sqrt " << an << " = " << bn << std::endl;
                stack.push(std::to_string(bn.getDoubleValue()));
            } else if (s.compare("pop") == 0) {
                stack.pop();
            } else if (s.compare("reverse") == 0) {
                stack.pop();
                std::string as = stack.top();
                
                Number an(as);
                
                // reverse next n (n is the value at the top of current stack) elements
                std::vector<std::string> temp;
                for (int i = 0; i < an.getIntegerValue() && stack.size() > 0; i++) {
                    temp.push_back(stack.top());
                    stack.pop();
                }
                
                for (auto token:temp) {
                    stack.push(token);
                }
            } else if (s.compare("repeat") == 0) {
                std::string as = stack.top();
                stack.pop();
                
                Number an(as);
                repeat = an.getIntegerValue();
            } else if (s.compare("endrepeat") == 0) {
                // it will never happen in this conditon, except unexpected unpaired 'repeat' and 'endrepeat'
            } else {
                stack.push(s);
            }
        }
    }
}


int main(int argc, char *argv[]) {
    // setup the print out format for the precision required.
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(3);
    
    // open the file for reading
    if (argc != 2) {
        throw std::invalid_argument("Need ONE data source file!");
    }
    std::ifstream in;
    in.open(argv[1]);
    
    // string to be read into
    std::string s;
    std::vector<std::string> inputVector;
    
    
    // read the file while we have input.
    while (in >> s) {
        inputVector.push_back(s);
    }
    in.close();
    
    handleToken(inputVector);
}
