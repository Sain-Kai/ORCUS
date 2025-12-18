#include "../include/orcus_aerothermal.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    // Shock-layer efficiency
    double shock_efficiency(double Mach) {
        if (Mach < 6.0) return 1.0;
        if (Mach <= 15.0)
            return 1.0 - 0.015 * (Mach - 6.0);
        return 0.865;
    }

    // Fay–Riddell real-gas enthalpy correction
    double fay_riddell_enthalpy_factor(
        double T_inf,
        double velocity
    ) {
        constexpr double CP_AIR = 1005.0;
        constexpr double H0_REF = 8.0e6; // J/kg (Mach ~8 reference)

        double h0 = CP_AIR * T_inf + 0.5 * velocity * velocity;
        return std::sqrt(std::max(h0 / H0_REF, 0.2));
    }

    // Wall catalysis
    double wall_catalysis_factor(double gamma_c) {
        return 1.0 + std::clamp(gamma_c, 0.0, 0.5);
    }

    // Laminar → turbulent transition heating
    double transition_heating_factor(
        double Re_x,
        double Re_crit
    ) {
        if (Re_x < Re_crit) return 1.0;
        return 3.0;
    }

}
