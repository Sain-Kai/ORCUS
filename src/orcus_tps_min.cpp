#include "../include/orcus_tps_min.h"
#include "../include/orcus_core.h"
#include "../include/orcus_envelope.h"
#include "../include/orcus_uncertainity.h"
#include "../include/orcus_config.h"
#include "../include/orcus_physics.h"

#include <iostream>
#include <vector>
#include <cmath>

namespace ORCUS {

    void run_minimum_tps_search() {

        std::cout << "\n====================================\n";
        std::cout << "ORCUS Phase-3Q — Minimum TPS Closure\n";
        std::cout << "====================================\n";

        OrcusConfig base_cfg = default_config();

        // Search space (engineering-reasonable)
        const std::vector<double> tps_candidates = {
            0.06, 0.08, 0.10, 0.12, 0.15, 0.18, 0.20
        };

        bool found = false;
        double selected = -1.0;

        for (double tps_thickness : tps_candidates) {

            OrcusConfig cfg = base_cfg;
            cfg.tps_thickness_m = tps_thickness;

            std::cout << "\n3Q | Testing TPS thickness = "
                      << tps_thickness << " m\n";

            // ---------- Phase-3K ----------
            ThermalSummary base =
                run_thermal_summary(cfg, 0.0);

            if (base.failure_mode != TPSFailureMode::NONE) {
                std::cout << "   → FAIL (3K baseline)\n";
                continue;
            }

            // ---------- Phase-3N ----------
            EnvelopeResult worst{};
            worst.peak_T_ratio = 0.0;
            worst.remaining_tps = 1.0;
            worst.survived = true;

            constexpr double gammas[] = { -5, -6, -7, -8 };
            constexpr double banks[]  = { 0, 30, 60 };
            constexpr double radii[]  = { 0.5, 1.0, 1.25 };

            for (double g : gammas) {
                for (double b : banks) {
                    for (double r : radii) {

                        OrcusConfig wc = cfg;
                        wc.initial_vz_mps =
                            wc.initial_speed_mps *
                            std::sin(g * 3.141592653589793 / 180.0);
                        wc.nose_radius_m = r;

                        ThermalSummary s =
                            run_thermal_summary(wc, b * 3.141592653589793 / 180.0);

                        worst.peak_T_ratio =
                            std::max(worst.peak_T_ratio, s.peak_T_ratio);

                        worst.remaining_tps =
                            std::min(worst.remaining_tps, s.remaining_tps);

                        if (s.failure_mode != TPSFailureMode::NONE)
                            worst.survived = false;
                    }
                }
            }

            if (!worst.survived) {
                std::cout << "   → FAIL (3N worst-case)\n";
                continue;
            }

            // ---------- Phase-3P ----------
            UncertaintyResult u =
                run_uncertainty_case(
                    density(30000.0),
                    base.peak_q,
                    base.peak_T_ratio * 2200.0,
                    tps_thickness,
                    2200.0,
                    { 0.15, 0.10, 0.20 }
                );

            if (!u.survived) {
                std::cout << "   → FAIL (3P uncertainty)\n";
                continue;
            }

            // ---------- SUCCESS ----------
            std::cout << "   ✔ SURVIVED ALL STAGES\n";
            selected = tps_thickness;
            found = true;
            break;
        }

        std::cout << "\n------------------------------------\n";
        if (found) {
            std::cout << "MINIMUM CERTIFIED TPS THICKNESS = "
                      << selected << " m\n";
        } else {
            std::cout << "NO TPS THICKNESS SURVIVED ALL PHASES\n";
        }
        std::cout << "------------------------------------\n";
    }

}
