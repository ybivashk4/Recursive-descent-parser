#include <iostream>
#include <fstream>
#include "lab3.h"
#include <exception>

int ParseAssignment::get(const std::string var){
   if (all_variables.find( var ) != all_variables.end()) {
        return all_variables[var];
   }
    std::cout << "Invalid syntax\n";
    exit(1);
    return INT32_MAX;
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
    
    cur_value = procE();
    if (cur_symb != ';') {
        std::cout << "Wrong syntax\n";
        exit(1);
    }
    set_var(cur_var, cur_value);
    os << cur_var << " = " << all_variables[cur_var] << std::endl;
    if (cur_symb == ';') {
        cur_value = 0;
        cur_var = cur_var.empty();
        cur_var = "";
        return;
    }


    
}

void ParseAssignment::start_parse() {
    while (!is.eof()){
        procS();
        while (!is.eof() && !isalpha(cur_symb) && cur_symb != '_') {
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
            std::cout << "Invalid Syntax\n";
            exit(1);
        }
    }
    if (all_variables.find(cur_var) == all_variables.end())
        set_var(cur_var, 0);
}

char ParseAssignment::procA() {
    if (isalpha(cur_symb) || cur_symb == '_') {
        char temp_symbol = cur_symb;
        read_symbol();
        return temp_symbol;
    }
    else {
        std::cout << "wrong syntax\n";
        exit(1);
    }
    read_symbol();
    return -1;
}

char ParseAssignment::procD() {
    if (is_o_digit(cur_symb) && !is.eof()) {
        char temp_symbol = cur_symb;
        read_symbol();
        return temp_symbol;
    }
    if (!is.eof())  read_symbol();
    return -1;
}

int ParseAssignment::procE() {
    int valE = procT();
    if (cur_symb == '+' || cur_symb == '-') {
        
        procE2(valE);
    }
    return valE;
    
}

void ParseAssignment::procE2(int & val) {
    int i = 10;
    while (cur_symb == '+' || cur_symb == '-') {
        if (cur_symb == '+') {
            read_symbol();
            val += procT();
        }
        else  {
            // ПОМЕНЯТЬ МЕСТАМИ ЕСЛИ НЕ РАБОТАЕТ
            read_symbol();
            val -= procT();
        }
        
        i--;
        if (i == 0) break;
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
            read_symbol();
            val *= procM();
        }
        else  {
            // ПОМЕНЯТЬ МЕСТАМИ ЕСЛИ НЕ РАБОТАЕТ
            read_symbol();
            val /= procM();
        }
    }
}

int ParseAssignment::procM() {
    // include porcI, procK, procC
    if (cur_symb == '(') {
        read_symbol();
        int temp = procE();
        if (cur_symb != ')') {
            std::cout << "() error\n";
            throw;
        }
        read_symbol();
        return temp;
    }
    else if (cur_symb == '-') {
        read_symbol();
        return procM() * -1;
    }
    else if (is_o_digit(cur_symb)){
        int temp_val = 0;
        std::string temp;
        while (!is.eof() && is_o_digit(cur_symb)) {
            temp += cur_symb;
            procD();
        }
        temp_val = std::stoi(temp);
        return temp_val;
    }
    else if (isalpha(cur_symb) || cur_symb == '_') {
        std::string temp;
        while (!is.eof() && (isalpha(cur_symb) || cur_symb == '_')) {
            temp += procA();
        }
        return get(temp);
    }
    return 0;
}

void ParseAssignment::getAllVar() {
    os << "\n\tALL results var is: \n";
    for (const auto & [key, value] : all_variables) {
        os << key << " = " << value << std::endl;
    }
}

int main (int argc, char ** argv) {
    ParseAssignment MyParse(argc, argv);
    MyParse.start_parse();
    return 0;
}