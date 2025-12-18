#pragma once

namespace ORCUS {

    struct StagnationField {
        double shock_standoff;   // m
        double p_stag;           // Pa
        double T_stag;           // K
        double rho_stag;         // kg/m^3
    };

    StagnationField compute_stagnation_field(
        double Mach_inf,
        double gamma,
        double R_n,
        double p_inf,
        double T_inf
    );

}
