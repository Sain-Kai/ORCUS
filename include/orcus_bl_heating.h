#pragma once
#include "orcus_boundary_layer.h"

namespace ORCUS {

    struct BLHeatFlux {
        double Stanton;   // Stanton number
        double q_wall;    // wall heat flux (W/m^2)
    };

    BLHeatFlux compute_bl_heat_flux(
        const BoundaryLayerProps& bl,
        double rho_e,
        double u_e,
        double cp,
        double T0_e,
        double T_wall
    );

}
