#pragma once

namespace ORCUS {

    struct NoneqChemistryProps {
        double dissociation_fraction;   // 0–1
        double relaxation_factor;       // 0–1
        double heat_multiplier;         // Final heat correction
    };

    // Computes nonequilibrium dissociation & relaxation effects
    NoneqChemistryProps compute_noneq_chemistry(
        double T_stag,    // Stagnation temperature (K)
        double Mach       // Freestream Mach number
    );

}
