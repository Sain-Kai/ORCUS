#include "../include/orcus_uncertainity.h"
#include <iostream>
#include <algorithm>

namespace ORCUS {

    UncertaintyResult run_uncertainty_case(
        double rho,
        double q_conv,
        double T_surface,
        double tps_thickness,
        double Tmax,
        const UncertaintyParams& u
    ) {
        UncertaintyResult r{};
        r.survived = true;

        double rho_eff = rho * (1.0 + u.rho_frac);
        double q_eff = q_conv * (1.0 + u.cd_frac) * (1.0 + u.heat_frac);

        double T_ratio = T_surface / Tmax;

        double remaining_frac = std::max(
            0.0,
            tps_thickness * (1.0 - u.heat_frac - u.cd_frac)
        ) / tps_thickness;

        r.worst_T_ratio = T_ratio;
        r.worst_remaining_tps = remaining_frac;

        if (T_ratio >= 1.0 || remaining_frac <= 0.0)
            r.survived = false;

        return r;
    }

    void run_uncertainty_analysis(
        double rho,
        double q_peak,
        double T_surface,
        double tps_thickness,
        double Tmax
    ) {
        std::cout << "\n====================================\n";
        std::cout << "ORCUS Phase-3P — Uncertainty Robustness Analysis\n";
        std::cout << "====================================\n";

        UncertaintyParams u{};
        u.rho_frac = 0.15;
        u.cd_frac = 0.10;
        u.heat_frac = 0.20;

        UncertaintyResult r =
            run_uncertainty_case(
                rho,
                q_peak,
                T_surface,
                tps_thickness,
                Tmax,
                u
            );

        std::cout << "--- Phase-3P Uncertainty Results ---\n";
        std::cout << "Survived              : " << r.survived << "\n";
        std::cout << "Worst T/Tmax          : " << r.worst_T_ratio << "\n";
        std::cout << "Worst remaining TPS   : " << r.worst_remaining_tps << "\n";
    }

}
