#pragma once

namespace ORCUS {

    enum class BoundaryLayerState {
        LAMINAR,
        TRANSITIONAL,
        TURBULENT
    };

    struct BoundaryLayerProps {
        double theta;        // momentum thickness (m)
        double delta_e;      // energy thickness (m)
        double Cf;           // skin friction coefficient
        double tau_w;        // wall shear stress (Pa)
        BoundaryLayerState state;
    };

    BoundaryLayerProps compute_boundary_layer(
        double rho_e,     // edge density
        double mu_e,      // edge viscosity
        double u_e,       // edge velocity
        double x,         // distance from stagnation (m)
        double Re_crit    // transition Reynolds number
    );

}
