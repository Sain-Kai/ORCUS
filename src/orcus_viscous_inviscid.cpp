#include "../include/orcus_viscous_inviscid.h"
#include <cmath>

namespace ORCUS {

    VICouplingResult solve_viscous_inviscid(
        double nose_radius,
        double delta_star,
        double Mach,
        double gamma
    ) {
        VICouplingResult r{};

        // Initial guesses
        double R_eff = nose_radius + delta_star;
        double Cp_old = 0.0;
        double Cp_new = 0.0;

        constexpr int MAX_IT = 20;
        constexpr double TOL = 1e-4;

        int it = 0;
        double err = 1.0;

        while (it < MAX_IT && err > TOL) {

            // Modified Newtonian Cp_max
            Cp_new =
                (2.0 / (gamma * Mach * Mach)) *
                (std::pow(
                    1.0 + 0.5 * (gamma - 1.0) * Mach * Mach,
                    gamma / (gamma - 1.0)
                ) - 1.0);

            // Geometry inflation feedback
            Cp_new *= (nose_radius / R_eff);

            err = std::abs(Cp_new - Cp_old);
            Cp_old = Cp_new;

            // Update effective radius
            R_eff = nose_radius + delta_star;

            it++;
        }

        r.effective_radius = R_eff;
        r.Cp_correction = Cp_new;
        r.convergence_error = err;
        r.iterations = it;

        return r;
    }

}
