#include "../include/orcus_surface_heat_distribution.h"
#include "../include/orcus_constants.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    SurfaceHeatDistribution compute_surface_heat_distribution(
        double q_stag,
        double nose_radius,
        int    n_points
    ) {
        SurfaceHeatDistribution dist{};
        dist.q_peak = q_stag;

        dist.points.reserve(n_points);

        // θ = 0 → stagnation
        // θ = π/2 → shoulder
        const double theta_max = PI / 2.0;

        for (int i = 0; i < n_points; ++i) {

            double frac = static_cast<double>(i) / (n_points - 1);
            double theta = frac * theta_max;

            // Arc length along nose
            double s = nose_radius * theta;

            // Modified Lees distribution
            double q_local =
                q_stag * std::pow(std::cos(theta), 1.5);

            dist.points.push_back({
                s,
                theta,
                q_local
                });
        }

        return dist;
    }

}
