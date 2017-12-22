#include <iostream>
#include <vector>
#include <map>
using namespace std;

enum PARSER_ERROR {
    UNEXPECTED_CHAR=1,
    END_OF_FILE,
    UNDECLARED_VARIABLE,
    UNEXPECTED_END_OF_FILE,
    DEF_NOT_CLOSED,
    INTERVAL_NOT_CLOSED,
    INVALID_RANGE
};

map <string, int> ASSIGNED_VALUES;

int string_to_int(const string& s) {
    if (s[0] == '-' or (s[0] >= '0' and s[0] <= '9')) return stoi(s);
    if (ASSIGNED_VALUES.find(s) != ASSIGNED_VALUES.end()) return ASSIGNED_VALUES[s];
    cerr << "ERROR: " << s << " is not declared" << endl;
    throw UNDECLARED_VARIABLE;
}

int random_interval(int a, int b) {
    if ( a > b ) throw INVALID_RANGE;
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
        if (c != '}') throw DEF_NOT_CLOSED;
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
        if (c != ')') throw INTERVAL_NOT_CLOSED;
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

        } catch (PARSER_ERROR e) {
            if (e == END_OF_FILE) throw UNEXPECTED_END_OF_FILE;
            throw e;
        }

        if (c != ']') throw UNEXPECTED_CHAR;

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
    if (!(cin >> c)) throw END_OF_FILE;

    Parser* p;

    if (c == '{') p = new Definition;
    else if (c == '(') p = new Interval;
    else if (c == '"') p = new StrConst;
    else if (c == '[') p = new Container;
    else throw UNEXPECTED_CHAR;

    p->read();
    return p;
}

int main () {
    srand(time(NULL));
    try {

        for (;;) {
            Parser* p = read_parser();
            p->generate();
            cout << endl;
        }

    } catch (PARSER_ERROR e) {
        if (e == END_OF_FILE) return 0;
        throw e;
    }
}
