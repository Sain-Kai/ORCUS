#pragma once

namespace ORCUS {

    struct RadiationProps {
        double emissive_power;     // Gas radiative emission (W/m^2)
        double absorption_factor;  // Gas absorption (0–1)
        double net_radiative_heat; // Net radiative heat to wall (W/m^2)
    };

    // Computes radiation–gas coupling in hypersonic shock layer
    RadiationProps compute_radiative_coupling(
        double T_gas,     // Shock-layer / stagnation temperature (K)
        double rho_gas,   // Gas density (kg/m^3)
        double path_len,  // Optical path length (m)
        double T_wall     // Wall temperature (K)
    );

}
