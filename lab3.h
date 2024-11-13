#include <string>
#include <map>
#include <exception>

class ParseAssignment {
    private:
        std::map <std::string, int> all_variables;
        char cur_symb;
        std::string cur_var;
        int cur_value;
        std::ifstream is;
        std::ofstream os;
        bool is_eof;
        void procS();
        void procIStr(); // add name of var
        void procE();
        void procE2();
        char procA();
        char procD();
        int procT();
        void procT2(int & val);
        int procM();
        static bool is_o_digit(char symbol);
        void read_symbol();
        void set_var(std::string name, int val);
    
    public:
        int get(const std::string var);
        void getAllVar();
        ParseAssignment(int argc, char ** argv);
        void start_parse();
};