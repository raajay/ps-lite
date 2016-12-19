#include <iostream>
#include "ps/ps.h"

int main() {
    ps::StartAsync();
    std::cout << "Started the param server" << std::endl;
    ps::Finalize();
    return 0;
}
