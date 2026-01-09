#pragma once

namespace ORCUS {

    struct DisplacementBL {
        double delta_star;   // displacement thickness (m)
        double shape_factor; // H = delta*/theta
    };

    // Computes displacement thickness from BL properties
    DisplacementBL compute_displacement_thickness(
        double theta,        // momentum thickness (m)
        double Cf,           // skin friction coefficient
        bool turbulent       // laminar / turbulent
    );

}
