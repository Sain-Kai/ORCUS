#include "../include/orcus_boundary_layer.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    BoundaryLayerProps compute_boundary_layer(
        double rho_e,
        double mu_e,
        double u_e,
        double x,
        double Re_crit
    ) {
        BoundaryLayerProps bl{};

        // -------------------------------
        // Reynolds number
        // -------------------------------
        double Re_x = rho_e * u_e * x / mu_e;

        // -------------------------------
        // Laminar skin friction
        // -------------------------------
        double Cf_lam = 0.664 / std::sqrt(std::max(Re_x, 1e3));

        // -------------------------------
        // Transition logic (integral-based)
        // -------------------------------
        if (Re_x < Re_crit) {
            bl.state = BoundaryLayerState::LAMINAR;
            bl.Cf = Cf_lam;
        }
        else {
            bl.state = BoundaryLayerState::TRANSITIONAL;
            bl.Cf = 2.5 * Cf_lam;  // conservative amplification
        }

        // -------------------------------
        // Momentum thickness
        // -------------------------------
        bl.theta =
            0.664 * x / std::sqrt(std::max(Re_x, 1e3));

        // -------------------------------
        // Energy thickness (hypersonic)
        // -------------------------------
        constexpr double Pr = 0.72;
        bl.delta_e = bl.theta * std::pow(Pr, -1.0 / 3.0);

        // -------------------------------
        // Wall shear stress
        // -------------------------------
        bl.tau_w =
            0.5 * rho_e * u_e * u_e * bl.Cf;

        return bl;
    }

}
