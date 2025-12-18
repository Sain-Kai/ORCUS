#include "../include/orcus_heat.h"
#include "../include/orcus_physics.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    // ===============================
    // Physical constants
    // ===============================
    static constexpr double SIGMA = 5.670374419e-8;   // Stefan–Boltzmann
    static constexpr double CP_AIR = 1005.0;          // J/kg-K
    static constexpr double C_FR = 1.83e-4;         // Sutton–Graves constant

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

        // Freestream temperature (from altitude physics)
        double T_inf = std::max(200.0, T_surface * 0.3);

        // Total (stagnation) enthalpy
        double h0 = CP_AIR * T_inf + 0.5 * velocity * velocity;
        double hw = CP_AIR * T_surface;

        // Convective heating (Fay–Riddell, enthalpy-limited)
        H.q_conv =
            C_FR *
            std::sqrt(std::max(rho, 1e-9) / std::max(nose_radius, 1e-4)) *
            std::pow(velocity, 3.0);

        // Enthalpy cap (prevents runaway)
        H.q_conv *= std::max(0.0, (h0 - hw) / h0);

        // Radiative heating (hypersonic)
        H.q_rad = 0.0;
        if (Mach > 10.0) {
            double M_eff = Mach - 10.0;
            double rad_gate = std::tanh(M_eff);  // smooth activation

            constexpr double C_RAD = 1.0e-7;
            H.q_rad =
                C_RAD *
                rad_gate *
                std::pow(rho, 0.5) *
                std::pow(velocity, 3.2);
        }

        // Surface re-radiation
        H.q_emit =
            emissivity *
            SIGMA *
            std::pow(T_surface, 4.0);

        // Net heat into TPS
        H.q_net =
            std::max(0.0, H.q_conv + H.q_rad - H.q_emit);

        return H;
    }

} // namespace ORCUS
