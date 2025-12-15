#pragma once

namespace ORCUS {

    struct MonteCarloStats {
        int runs;
        int survived;
        int temp_failures;
        int ablation_failures;

        double survival_rate;
        double worst_T_ratio;
        double worst_remaining_tps;
    };

    void run_montecarlo_certification();

}
