#pragma once

namespace ORCUS {

    struct VICouplingResult {
        double effective_radius;   // geometry inflated by BL
        double Cp_correction;      // pressure correction factor
        double convergence_error;  // coupling residual
        int iterations;            // iterations taken
    };

    // Viscous–inviscid coupling solver
    VICouplingResult solve_viscous_inviscid(
        double nose_radius,
        double delta_star,
        double Mach,
        double gamma
    );

}
