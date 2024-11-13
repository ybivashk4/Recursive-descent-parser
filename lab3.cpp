#include <iostream>
#include <fstream>
#include "lab3.h"
#include <exception>

int ParseAssignment::get(const std::string var){
   
    try {
        return all_variables[var];
    }
    catch (std::exception e) {
        
        std::cout << e.what();
        std::cout << "Error in " << var << " because this var doesn't exist\n";
    }
}

void ParseAssignment::set_var(std::string name, int val) {
    all_variables[name] = val;
}

ParseAssignment::ParseAssignment(int argc, char ** argv){
    if (argc > 2) {
        is.open(argv[1], std::ios_base::in);
        os.open(argv[2], std::ios_base::out);
        if (!is.is_open() || !os.is_open()) {
            std::cout << "can't open file" << std::endl;
            throw;
        }
    }
    else {
        std::cout << "wrong input params\n";
        throw;
    }
    read_symbol();
    cur_value = 0;
    
}

void ParseAssignment::read_symbol() {
    is >> cur_symb;
}

bool ParseAssignment::is_o_digit(char symbol) {
    return isdigit(symbol) && symbol != '8' && symbol != '9';
}

void ParseAssignment::procS() {
    procIStr();
    
    if (cur_symb != '=') {
        std::cout << "error, = missing\n";
        throw;
    }
    read_symbol();
    procE();
    if (cur_symb != ';' || cur_symb != is.eof()) {
        cur_value = 0;
        cur_var.empty();
        return;
    }


    
}

void ParseAssignment::start_parse() {
    while (cur_symb != is.eof()){
        procS();
        
        while (cur_symb != is.eof() && (!isalpha(cur_symb) && cur_symb != '_')) {
            printf("cur symb is %c\n", cur_symb);
            read_symbol();
        }
    }
    
    getAllVar();
}

void ParseAssignment::procIStr() {
    
    cur_var += procA();

    while ((isalpha(cur_symb) || cur_symb == '_') || is_o_digit(cur_symb)) {

        if (isalpha(cur_symb) || cur_symb == '_') {
            cur_var += procA();
        }
        else if (is_o_digit(cur_symb)) {
            cur_var += procD();
        }
        else if (cur_symb == '=') {
            break;
        }
        else if (cur_symb == is.eof()) {
            throw;
        }
        else {
            break;
        }
    }
    set_var(cur_var, 0);
}

char ParseAssignment::procA() {
    if (isalpha(cur_symb) || cur_symb == '_') {
        char temp_symbol = cur_symb;
        read_symbol();
        return temp_symbol;
    }
    read_symbol();
    return -1;
}

char ParseAssignment::procD() {
    if (is_o_digit(cur_symb)) {
        char temp_symbol = cur_symb;
        read_symbol();
        return temp_symbol;
    }
    return -1;
}

void ParseAssignment::procE() {
    cur_value = procT();
    
    if (cur_symb == '+' || cur_symb == '-') {
        procE2();
    }
    set_var(cur_var, cur_value);
}

void ParseAssignment::procE2() {
    while (cur_symb == '+' || cur_symb == '-') {
        if (cur_symb == '+') {
            cur_value += procT();
        }
        else  {
            // ПОМЕНЯТЬ МЕСТАМИ ЕСЛИ НЕ РАБОТАЕТ
            cur_value -= procT();
        }
    }
}

int ParseAssignment::procT() {
    int valT = procM();
    if (cur_symb == '*' || cur_symb == '/') {
        procT2(valT);
    }
    return valT;
}

void ParseAssignment::procT2(int & val) {
    while (cur_symb == '*' || cur_symb == '/') {
        if (cur_symb == '*') {
            val *= procM();
        }
        else  {
            // ПОМЕНЯТЬ МЕСТАМИ ЕСЛИ НЕ РАБОТАЕТ
            val /= procM();
        }
    }
}

int ParseAssignment::procM() {
    // include porcI, procK, procC
    if (cur_symb == '(') {
        read_symbol();
        procE();
        if (cur_symb != ')') {
            std::cout << "() error\n";
            throw;
        }
    }
    else if (cur_symb == '-') {
        read_symbol();
        return procM() * -1;
    }
    else if (is_o_digit(cur_symb)){
        int temp_val = 0;
        bool is_first_digit = true;
        std::string temp;
        while (is_o_digit(cur_symb)) {
            temp += cur_symb;
            procD();
        }
        temp_val = std::stoi(temp);
        return temp_val;
    }
    else if (isalpha(cur_symb) || cur_symb == '_') {
        std::string temp;
        while (isalpha(cur_symb) || cur_symb == '_') {
            temp += procA();
        }
        return get(temp);
    }
}

void ParseAssignment::getAllVar() {
    for (const auto & [key, value] : all_variables) {
        std::cout << key << " = " << value << "\n";
    }
}

int main (int argc, char ** argv) {
    std::ifstream is;
    is.open(argv[1], std::ios_base::in);

    ParseAssignment MyParse(argc, argv);
    MyParse.start_parse();
    return 0;
}