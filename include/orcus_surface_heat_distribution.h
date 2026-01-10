#pragma once
#include <vector>

namespace ORCUS {

    struct SurfacePointHeat {
        double s;        // Surface arc length (m)
        double theta;    // Local surface angle (rad)
        double q_local;  // Local heat flux (W/m^2)
    };

    struct SurfaceHeatDistribution {
        std::vector<SurfacePointHeat> points;
        double q_peak;
    };

    // -------------------------------------------
    // Phase-6B: Distribute stagnation heat flux
    // over blunt body surface
    // -------------------------------------------
    SurfaceHeatDistribution compute_surface_heat_distribution(
        double q_stag,        // stagnation heat flux (W/m^2)
        double nose_radius,   // nose radius (m)
        int    n_points       // surface discretization
    );

} // namespace ORCUS
