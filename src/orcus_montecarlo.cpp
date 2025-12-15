#include "../include/orcus_montecarlo.h"
#include "../include/orcus_core.h"
#include "../include/orcus_config.h"
#include "../include/orcus_constants.h"

#include <iostream>
#include <random>
#include <algorithm>

namespace ORCUS {

    void run_montecarlo_certification() {

        std::cout << "\n====================================\n";
        std::cout << "ORCUS Phase-3W — Monte-Carlo Certification\n";
        std::cout << "====================================\n";

        constexpr int N = 1000;

        MonteCarloStats stats{};
        stats.runs = N;
        stats.worst_T_ratio = 0.0;
        stats.worst_remaining_tps = 1.0;

        std::mt19937 rng(42);

        // --- Dispersions (certification realistic) ---
        std::uniform_real_distribution<double> gamma_disp(-7.0, -5.0);
        std::uniform_real_distribution<double> bank_disp(15.0, 60.0);
        std::uniform_real_distribution<double> rho_scale(0.85, 1.15);

        OrcusConfig base_cfg = default_config();
        base_cfg.tps_thickness_m = 0.12;   // LOCKED (Phase-3Q)

        for (int i = 0; i < N; ++i) {

            OrcusConfig cfg = base_cfg;

            double gamma = gamma_disp(rng);
            double bank  = bank_disp(rng);

            cfg.initial_vz_mps =
                cfg.initial_speed_mps *
                std::sin(gamma * PI / 180.0);

            ThermalSummary s =
                run_thermal_summary(cfg, bank * PI / 180.0);

            stats.worst_T_ratio =
                std::max(stats.worst_T_ratio, s.peak_T_ratio);

            stats.worst_remaining_tps =
                std::min(stats.worst_remaining_tps, s.remaining_tps);

            if (s.failure_mode == TPSFailureMode::NONE) {
                stats.survived++;
            }
            else if (s.failure_mode == TPSFailureMode::OVER_TEMP) {
                stats.temp_failures++;
            }
            else if (s.failure_mode == TPSFailureMode::ABLATION) {
                stats.ablation_failures++;
            }
        }

        stats.survival_rate =
            static_cast<double>(stats.survived) / stats.runs;

        // -------- Results --------
        std::cout << "\n--- Monte-Carlo Results ---\n";
        std::cout << "Total runs           : " << stats.runs << "\n";
        std::cout << "Survived             : " << stats.survived << "\n";
        std::cout << "Temp failures        : " << stats.temp_failures << "\n";
        std::cout << "Ablation failures    : " << stats.ablation_failures << "\n";
        std::cout << "Survival rate        : "
                  << stats.survival_rate * 100.0 << " %\n";
        std::cout << "Worst T/Tmax         : " << stats.worst_T_ratio << "\n";
        std::cout << "Worst remaining TPS  : " << stats.worst_remaining_tps << "\n";

        std::cout << "\n====================================\n";
        std::cout << "Phase-3W completed: CERTIFICATION MATRIX CLOSED\n";
        std::cout << "====================================\n";
    }

}
