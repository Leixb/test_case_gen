#include <iostream>
#include "maker.h"

int main () {

    // Random device seed
    srand(time(NULL));

    try {

        for (;;) {
            Parser* p = read_parser(std::cin);
            p->generate(std::cout);
            std::cout << std::endl;
        }

    } catch (const unexpected_char& e) {
        std::cerr << e.what() << std::endl
            << "Expected '" << e.expect()
            << "' but got '" << e.got() << '\'' << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

}
