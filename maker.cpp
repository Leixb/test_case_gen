#include <iostream>
#include <vector>
#include <map>
#include <exception>
#include <cstdio>
using namespace std;

class end_of_file : public exception {
    const char* what() const noexcept {
        return "Reached end of file";
    }
};

class unexpected_char : public exception {
    char expected, c;

    public:

    unexpected_char(const char& expected, const char& c): expected(expected), c(c) { }

    const char* what() const noexcept {
        return "Unexpected character";
    }
};

class undeclared_variable : public exception {

    string variable;

    public:

    undeclared_variable(const string& variable): variable(variable) {}

    const char* what() const noexcept {
        return "Undeclared variable";
    }
};

template<typename T>
class invalid_range : public exception {
    T lo, hi;

    public:

    invalid_range(const T& lo, const T& hi): lo(lo), hi(hi) {}

    const char* what() const noexcept {
        return "Invalid range";
    }

};

map <string, int> ASSIGNED_VALUES;

int string_to_int(const string& s) {
    if (s[0] == '-' or (s[0] >= '0' and s[0] <= '9')) return stoi(s);
    if (ASSIGNED_VALUES.find(s) != ASSIGNED_VALUES.end()) return ASSIGNED_VALUES[s];
    cerr << "ERROR: " << s << " is not declared" << endl;
    throw undeclared_variable(s);
}

template<typename T>
T random_interval(T a, T b) {
    if (a > b) throw invalid_range<T>(a, b);
    return a + (rand()%(b - a + 1));
}

int random_interval(const string& a, const string& b) {
    return random_interval(
            string_to_int(a),
            string_to_int(b)
            );
}

class Parser {
    public:
        virtual void read() = 0;
        virtual void generate() = 0;
};

// { name | a b }
class Definition : public Parser {
    string name;
    string low, hi;

    int current;

    public:

    void read() {
        char c;
        cin >> name >> c >> low >> hi >> c;
        if (c != '}') throw unexpected_char('}', c);
    }

    void generate() {
        ASSIGNED_VALUES[name] = current = random_interval(low, hi);
        if (name[0] != '_') cout << current << ' ';
    }
};

// ( repetitons | a b )
class Interval : public Parser {

    string repetitons, low, hi;

    public:

    void read() {
        char c;
        cin >> repetitons >> c >> low >> hi >> c;
        if (c != ')') throw unexpected_char(')', c);
    }

    void generate() {
        for (int i = 0; i < string_to_int(repetitons); ++i) {
            cout << random_interval(low, hi) << ' ';
        }
    }

};

// "s fsd fsd fs fds s"
class StrConst : public Parser {

    string content;

    public:

    void read() {
        char c;
        while (cin >> noskipws >> c and c != '"') content.push_back(c);
        if (c != '"') throw end_of_file();
        cin >> skipws;
    }

    void generate() {
        cout << content;
    }

};

Parser* read_parser();

// [ repetitions | ( ... ), [...] ]
class Container : public Parser {

    string repetitions;

    vector<Parser*> content;

    public:

    void read() {
        char c;

        cin >> repetitions >> c;

        try {

            do {
                Parser* value = read_parser();
                content.push_back(value);
            } while (cin >> c and c == ',');

        } catch (const end_of_file& e) { }

        if (c != ']') throw unexpected_char(']', c);

    }

    void generate() {
        for (int i = 0; i < string_to_int(repetitions); ++i) {
            for (Parser *value : content) value->generate();
            cout << endl;
        }
    }

};

Parser* read_parser() {
    char c;

    Parser* p = NULL;

    while (p == NULL) {
        if (!(cin >> c)) throw end_of_file();

        if (c == '{') p = new Definition;
        else if (c == '(') p = new Interval;
        else if (c == '"') p = new StrConst;
        else if (c == '[') p = new Container;
    }

    p->read();
    return p;
}

int main () {

    // Random device seed
    srand(time(NULL));

    try {

        for (;;) {
            Parser* p = read_parser();
            p->generate();
            cout << endl;
        }

    } catch (const exception& e) {
        cerr << e.what() << endl;
    }
}
