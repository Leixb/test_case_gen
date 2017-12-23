#include <iostream>
#include "maker.h"
using namespace std;

int main () {

    // Random device seed
    srand(time(NULL));

    try {

        for (;;) {
            Parser* p = read_parser(cin);
            p->generate(cout);
            cout << endl;
        }

    } catch (const unexpected_char& e) {
        cerr << e.what() << endl
            << "Expected '" << e.expect()
            << "' but got '" << e.got() << '\'' << endl;
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }

}
