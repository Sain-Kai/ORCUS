#include "../include/orcus_heat.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    // Physical constants
    static constexpr double SIGMA = 5.670374419e-8;   // Stefan–Boltzmann
    static constexpr double CP_AIR = 1005.0;          // J/kg-K (air)
    static constexpr double T_INF = 220.0;            // Upper-atmosphere ref
    static constexpr double C_FR = 1.83e-4;           // Fay–Riddell constant

    HeatFlux compute_heating(
        double rho,
        double velocity,
        double nose_radius,
        double gamma,
        double Mach,
        double Re_x,
        double T_surface,
        double emissivity
    ) {
        HeatFlux H{};

        // -------------------------------
        // Stagnation enthalpy (total)
        // -------------------------------
        double h0 = CP_AIR * T_INF + 0.5 * velocity * velocity;
        double hw = CP_AIR * T_surface;

        // -------------------------------
        // Convective heating (enthalpy-limited)
        // Fay–Riddell style
        // -------------------------------
        H.q_conv =
            C_FR *
            std::sqrt(std::max(rho, 1e-9) / std::max(nose_radius, 1e-4)) *
            std::max(h0 - hw, 0.0);

        // -------------------------------
        // Radiative heating (hypersonic)
        // Active beyond Mach ~10
        // -------------------------------
        H.q_rad = 0.0;
        if (Mach > 10.0) {
            double T_shock = (h0/CP_AIR)*(0.9 + 0.1 * std::tanh(Mach - 10.0));
			constexpr double C_rad = 1.2e-8;
            H.q_rad =
                C_rad *
                std::pow(rho,0.5)*
                std::pow(T_shock, 4.0);
        }

        // -------------------------------
        // Surface re-radiation
        // -------------------------------
        H.q_emit =
            emissivity *
            SIGMA *
            std::pow(T_surface, 4.0);

        // -------------------------------
        // Net heat into TPS
        // -------------------------------
        H.q_net =
            std::max(0.0, H.q_conv + H.q_rad - H.q_emit);

        return H;
    }

} // namespace ORCUS
