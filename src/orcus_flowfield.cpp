#include "../include/orcus_flowfield.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    StagnationField compute_stagnation_field(
        double Mach_inf,
        double gamma,
        double R_n,
        double p_inf,
        double T_inf
    ) {
        StagnationField s{};

        // ----------------------------------------
        // 1. Shock stand-off distance
        // ----------------------------------------
        double denom = std::sqrt(std::max(gamma * Mach_inf * Mach_inf - 1.0, 1e-6));
        s.shock_standoff = R_n / denom;

        // ----------------------------------------
        // 2. Normal shock relations (stagnation line)
        // ----------------------------------------

        // Pressure ratio across normal shock
        double p2_p1 =
            1.0 +
            (2.0 * gamma / (gamma + 1.0)) *
            (Mach_inf * Mach_inf - 1.0);

        double p2 = p_inf * p2_p1;

        // Density ratio
        double rho2_rho1 =
            ((gamma + 1.0) * Mach_inf * Mach_inf) /
            ((gamma - 1.0) * Mach_inf * Mach_inf + 2.0);

        double T2_T1 = p2_p1 / rho2_rho1;
        double T2 = T_inf * T2_T1;

        // Post-shock Mach number
        double M2_sq =
            (1.0 + 0.5 * (gamma - 1.0) * Mach_inf * Mach_inf) /
            (gamma * Mach_inf * Mach_inf - 0.5 * (gamma - 1.0));

        double M2 = std::sqrt(std::max(M2_sq, 1e-6));

        // ----------------------------------------
        // 3. Isentropic deceleration to stagnation
        // ----------------------------------------
        double stag_factor =
            std::pow(
                1.0 + 0.5 * (gamma - 1.0) * M2 * M2,
                gamma / (gamma - 1.0)
            );

        s.p_stag = p2 * stag_factor;
        s.T_stag = T2 * (1.0 + 0.5 * (gamma - 1.0) * M2 * M2);
        s.rho_stag = s.p_stag / (287.0 * s.T_stag);

        return s;
    }

}
