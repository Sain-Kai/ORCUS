#include "../include/orcus_core.h"
#include "../include/orcus_envelope.h"
#include "../include/orcus_uncertainity.h"

#include <iostream>

int main() {

    std::cout << "ORCUS Phase-3 Engine Running\n";

    ORCUS::run_default_simulation();   // Phase-3K / 3M
    return 0;
}
