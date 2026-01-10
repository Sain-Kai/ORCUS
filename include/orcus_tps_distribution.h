#pragma once
#include <vector>

namespace ORCUS {

    struct TPSTile {
        double x;                // position along body (m)
        double heat_flux;        // W/m^2
        double thickness_remain; // m
        bool failed;
    };

    struct TPSDistributionResult {
        std::vector<TPSTile> tiles;
        double min_thickness;
        double failure_x;
        bool any_failure;
    };

    TPSDistributionResult compute_tps_distribution(
        double initial_thickness,
        double density,
        double heat_of_ablation,
        double time_exposure,
        const std::vector<double>& x,
        const std::vector<double>& q
    );

}
