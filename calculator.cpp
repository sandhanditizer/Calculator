/*
Zachariah Kline
20 Oct 2020
Calculator
*/

#include <iostream>
#include "tokens.h"
#include <random>
using namespace std;

Token_stream ts;

double expression();
double term();
double primary();
double rand_double();
void calculator();

int main() {
    try {
        cout << "*Calculator*\t" << "(';' to print and 'q' to quit)\n\n";
        cout << "> ";
        define_name("e", 2.718281828459045);
        define_name("pi", 3.1415926535897932);
        calculator();
        return 0;
    }
    catch (runtime_error& e) {
        cerr << e.what() << '\n';
        return 1;
    }
    catch (exception& e) {
        cerr << e.what() << '\n';
        return 2;
    }
}

void calculator() {  // calculating users input 
    double val = 0;
    while (cin) {
        Token t = ts.get();
        if (t.kind == quit) {
            break;
        }
        if (t.kind == print) {
            cout << "= " << val << '\n';
            cout << "> ";
        }
        else {
            ts.putback(t);
            val = expression();
        }
    }
}

double rand_double() {  // random number/answer generator
    static random_device rd;
    static default_random_engine engine{ rd() };
    static uniform_real_distribution<double> dist{ -10000, 10000 };
    return dist(engine);
}

double expression() {
    double left = term();
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
        case '+':
            left += term();
            t = ts.get();
            break;
        case '-':
            left -= term();
            t = ts.get();
            break;
        default:
            ts.putback(t);
            return left;
        }
    }
}

double term() {
    double left = primary();
    Token t = ts.get();
    while (true) {
        switch (t.kind) {
        case '*':
            left *= primary();
            t = ts.get();
            break;
        case '/': {
            double right = primary();
            if (right == 0) {
                throw runtime_error("Indeterminate Form: n/0\n");
            }
            left /= right;
            t = ts.get();
            break;
        }
        default:
            ts.putback(t);
            return left;
        }
    }
}

double primary() {
    Token t = ts.get();
    switch (t.kind) {
    case '(':
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')') {
            throw runtime_error("')' expected");
        }
        return d;
    }
        break;
    case number:
        return t.value;
        break;
    case name:
        return get_value(t.name);
        break;
    case '+':
        return primary();
        break;
    case '-':
        return -primary();
    case '?':
        return rand_double();
        break;
    default:
        throw runtime_error("Primary expected instead of: " + string{ t.kind });
    }
}
/*  
Expression:
    Term
    Expression + Term
    Expression - Term
Term:
    Primary
    Term * Primary
    Term / Primary
 Primary:
    Number
    Name (variables)
    "(" Expression ")"
    "-" Primary  (for negative numbers)
    "+" Primary  (for positive numbers)
    "?"   (for random value answers)
*/