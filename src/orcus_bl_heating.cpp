#include "../include/orcus_bl_heating.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    BLHeatFlux compute_bl_heat_flux(
        const BoundaryLayerProps& bl,
        double rho_e,
        double u_e,
        double cp,
        double T0_e,
        double T_wall
    ) {
        BLHeatFlux q{};

        constexpr double Pr = 0.72;

        // Stanton number (laminar baseline)
        double St_lam =
            (bl.Cf / 2.0) * std::pow(Pr, -2.0 / 3.0);

        // Turbulent / transitional boost
        if (bl.state != BoundaryLayerState::LAMINAR) {
            q.Stanton = 3.0 * St_lam;
        }
        else {
            q.Stanton = St_lam;
        }

        // Wall heat flux
        q.q_wall =
            q.Stanton *
            rho_e *
            u_e *
            cp *
            std::max(T0_e - T_wall, 0.0);

        return q;
    }

}
