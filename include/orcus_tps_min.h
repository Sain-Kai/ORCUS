#pragma once

namespace ORCUS {

    struct TPSMinResult {
        double thickness;
        bool survived;
    };

    void run_minimum_tps_search();

}
