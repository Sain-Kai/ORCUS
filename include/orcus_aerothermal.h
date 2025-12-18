#pragma once

namespace ORCUS {

    // -------------------------------
    // Phase-4B correction factors
    // -------------------------------

    double shock_efficiency(double Mach);

    double fay_riddell_enthalpy_factor(
        double T_inf,
        double velocity
    );

    double wall_catalysis_factor(double gamma_c);

    double transition_heating_factor(
        double Re_x,
        double Re_crit
    );

}
