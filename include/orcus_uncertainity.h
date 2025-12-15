#pragma once

namespace ORCUS {

    struct UncertaintyParams {
        double rho_frac;
        double cd_frac;
        double heat_frac;
    };

    struct UncertaintyResult {
        bool survived;
        double worst_T_ratio;
        double worst_remaining_tps;
    };

    UncertaintyResult run_uncertainty_case(
        double rho,
        double q_conv,
        double T_surface,
        double tps_thickness,
        double Tmax,
        const UncertaintyParams& u
    );

    // Phase-3P entry point
    void run_uncertainty_analysis(
        double rho,
        double q_peak,
        double T_surface,
        double tps_thickness,
        double Tmax
    );

}
