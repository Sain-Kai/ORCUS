#include "../include/orcus_tps_distribution.h"
#include <algorithm>

namespace ORCUS {

    TPSDistributionResult compute_tps_distribution(
        double initial_thickness,
        double density,
        double heat_of_ablation,
        double time_exposure,
        const std::vector<double>& x,
        const std::vector<double>& q
    ) {
        TPSDistributionResult res{};
        res.min_thickness = initial_thickness;
        res.any_failure = false;
        res.failure_x = -1.0;

        for (size_t i = 0; i < x.size(); ++i) {

            double m_dot = q[i] / heat_of_ablation;
            double dh = (m_dot * time_exposure) / density;

            double h_remain = initial_thickness - dh;
            bool failed = (h_remain <= 0.0);

            res.tiles.push_back({
                x[i],
                q[i],
                std::max(0.0, h_remain),
                failed
                });

            res.min_thickness =
                std::min(res.min_thickness, h_remain);

            if (failed && !res.any_failure) {
                res.any_failure = true;
                res.failure_x = x[i];
            }
        }

        return res;
    }

}
