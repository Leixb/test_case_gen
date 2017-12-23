#ifndef __parser_included__
#define __parser_included__

#include <iostream>
#include <vector>
#include <map>
#include <exception>

class end_of_file : public std::exception {
    bool premature;

    public:

    end_of_file(bool premature=false): premature(premature) { }

    const char* what() const noexcept {
        if (premature) return "Prematurely reached end of file";
        return "Reached EOF";
    }

    bool is_premature() const { return premature; }
};

class unexpected_char : public std::exception {
    char expected, c;

    public:

    unexpected_char(const char& expected, const char& c): expected(expected), c(c) { }

    const char* what() const noexcept {
        return "Unexpected character";
    }

    char expect() const { return expected; }
    char got() const { return c; }
};

class undeclared_variable : public std::exception {

    std::string variable;

    public:

    undeclared_variable(const std::string& variable): variable(variable) {}

    const char* what() const noexcept {
        return "Undeclared variable";
    }

    std::string name() const { return variable; }
};

template<typename T>
class invalid_range : public std::exception {
    T lo, hi;

    public:

    invalid_range(const T& lo, const T& hi): lo(lo), hi(hi) {}

    const char* what() const noexcept {
        return "Invalid range";
    }

    T low() const { return lo; }
    T high() const { return hi; }

};

std::map <std::string, int> ASSIGNED_VALUES;

int string_to_int(const std::string& s) {
    if (s[0] == '-' or (s[0] >= '0' and s[0] <= '9')) return stoi(s);
    if (ASSIGNED_VALUES.find(s) != ASSIGNED_VALUES.end()) return ASSIGNED_VALUES[s];
    throw undeclared_variable(s);
}

template<typename T>
T random_interval(T a, T b) {
    if (a > b) throw invalid_range<T>(a, b);
    return a + (rand()%(b - a + 1));
}

int random_interval(const std::string& a, const std::string& b) {
    return random_interval(
            string_to_int(a),
            string_to_int(b)
            );
}

class Parser {
    public:
        virtual void read(std::istream&) = 0;
        virtual void generate(std::ostream&) = 0;
};

// { name | a b }
class Definition : public Parser {
    std::string name;
    std::string low, hi;

    int current;

    public:

    void read(std::istream& in) {
        char c;
        in >> name >> c >> low >> hi >> c;
        if (c != '}') throw unexpected_char('}', c);
    }

    void generate(std::ostream& out) {
        ASSIGNED_VALUES[name] = current = random_interval(low, hi);
        if (name[0] != '_') out << current << ' ';
    }
};

// ( repetitons | a b )
class Interval : public Parser {

    std::string repetitons, low, hi;

    public:

    void read(std::istream& in) {
        char c;
        in >> repetitons >> c >> low >> hi >> c;
        if (c != ')') throw unexpected_char(')', c);
    }

    void generate(std::ostream& out) {
        for (int i = 0; i < string_to_int(repetitons); ++i) {
            if (i) out << ' ';
            out << random_interval(low, hi);
        }
    }

};

// "s fsd fsd fs fds s"
class StrConst : public Parser {

    std::string content;

    public:

    void read(std::istream& in) {
        char c;
        while (in >> std::noskipws >> c and c != '"') content.push_back(c);
        if (c != '"') throw end_of_file();
        in >> std::skipws;
    }

    void generate(std::ostream& out) {
        out << content;
    }

};

Parser* read_parser(std::istream&);

// [ repetitions | ( ... ), [...] ]
class Container : public Parser {

    std::string repetitions;

    std::vector<Parser*> content;

    public:

    void read(std::istream& in) {
        char c;

        std::cin >> repetitions >> c;

        try {

            do {
                Parser* value = read_parser(in);
                content.push_back(value);
            } while (in >> c and c == ',');

        } catch (const end_of_file& e) {
            throw end_of_file(true);
        }

        if (c != ']') throw unexpected_char(']', c);

    }

    void generate(std::ostream& out) {
        for (int i = 0; i < string_to_int(repetitions); ++i) {
            for (Parser *value : content) value->generate(out);
            out << std::endl;
        }
    }

};

Parser* read_parser(std::istream& in) {
    char c;

    Parser* p = NULL;

    while (p == NULL) {
        if (!(in >> c)) throw end_of_file();

        if (c == '{') p = new Definition;
        else if (c == '(') p = new Interval;
        else if (c == '"') p = new StrConst;
        else if (c == '[') p = new Container;
    }

    p->read(in);
    return p;
}

#endif
