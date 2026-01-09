#include "../include/orcus_radiation.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    RadiationProps compute_radiative_coupling(
        double T_gas,
        double rho_gas,
        double path_len,
        double T_wall
    ) {
        RadiationProps r{};

        constexpr double SIGMA = 5.670374419e-8; // Stefan–Boltzmann

        // --------------------------------------------------
        // Gas emissive power (simplified high-enthalpy model)
        // Strong T^4 dependence, density weighted
        // --------------------------------------------------
        r.emissive_power =
            SIGMA *
            std::pow(T_gas, 4.0) *
            std::sqrt(std::max(rho_gas, 1e-6));

        // --------------------------------------------------
        // Absorption factor (Beer–Lambert type)
        // --------------------------------------------------
        double kappa =
            0.15 * std::sqrt(std::max(rho_gas, 1e-6)); // absorption coeff

        r.absorption_factor =
            1.0 - std::exp(-kappa * path_len);

        r.absorption_factor =
            std::clamp(r.absorption_factor, 0.0, 1.0);

        // --------------------------------------------------
        // Net radiative heat to wall
        // --------------------------------------------------
        double wall_emission =
            SIGMA * std::pow(T_wall, 4.0);

        r.net_radiative_heat =
            r.absorption_factor *
            std::max(r.emissive_power - wall_emission, 0.0);

        return r;
    }

}
