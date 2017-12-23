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
    } catch (const end_of_file& e) {
        if (e.is_premature()) cerr << e.what() << endl;
    } catch (const invalid_range<int>& e) {
        cerr << e.what() << endl
            << e.low() << " is bigger than " << e.high() << endl;
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }
}
