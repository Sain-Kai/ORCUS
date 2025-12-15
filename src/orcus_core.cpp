#include "../include/orcus_core.h"
#include "../include/orcus_dynamics.h"
#include "../include/orcus_physics.h"
#include "../include/orcus_heat.h"
#include "../include/orcus_tps.h"
#include "../include/orcus_guidance.h"
#include "../include/orcus_constants.h"
#include "../include/orcus_envelope.h"
#include "../include/orcus_config.h"
#include "../include/orcus_uncertainity.h"
#include "../include/orcus_tps_min.h"
#include "../include/orcus_montecarlo.h"

#include <iostream>
#include <cmath>
#include <algorithm>

namespace ORCUS {

    // ===============================
    // Phase banner utility
    // ===============================
    void print_stage_banner(OrcusStage stage) {
        std::cout << "\n====================================\n";
        switch (stage) {
        case OrcusStage::PHASE_3K:
            std::cout << "ORCUS Phase-3K — Thermal Margin Quantification\n";
            break;
        case OrcusStage::PHASE_3N:
            std::cout << "ORCUS Phase-3N — Worst-Case Envelope\n";
            break;
        case OrcusStage::PHASE_3P:
            std::cout << "ORCUS Phase-3P — Uncertainty Robustness Analysis\n";
            break;
        case OrcusStage::PHASE_3O:
            std::cout << "ORCUS Phase-3O — Certification Traceability\n";
            break;
        case OrcusStage::PHASE_3Q:
            std::cout << "ORCUS Phase-3Q — Minimum TPS Closure\n";
            break;
        case OrcusStage::PHASE_3W:
            std::cout << "ORCUS Phase-3W — Monte-Carlo Certification\n";
            break;


        }
        std::cout << "====================================\n";
    }

    // ==================================================
    // Core thermal evaluation (Phase-3K / 3N / 3P base)
    // ==================================================
    ThermalSummary run_thermal_summary(
        const OrcusConfig& cfg,
        double bank_rad
    ) {
        ThermalSummary out{};
        out.peak_T_ratio = 0.0;
        out.remaining_tps = 1.0;
        out.peak_q = 0.0;
        out.failure_mode = TPSFailureMode::NONE;

        Vehicle6DOF veh{};
        veh.mass = 1500.0;
        veh.Sref = 1.8;
        veh.cref = 2.0;
        veh.bank = bank_rad;
        veh.Ixx = 800;
        veh.Iyy = 1200;
        veh.Izz = 1000;

        TPSMaterial tps{ 1600.0, 1250.0, 0.15, 0.9, 2200.0, 2.5e6 };

        TPSState tps_state{};
        tps_state.T_surface = 300.0;
        tps_state.T_bulk = 300.0;
        tps_state.thickness = cfg.tps_thickness_m;
        tps_state.failed = false;
        tps_state.failure_mode = TPSFailureMode::NONE;

        State6DOF s{};
        s.x = 0.0;
        s.y = 0.0;
        s.z = cfg.initial_altitude_m;
        s.vx = cfg.initial_speed_mps;
        s.vy = 0.0;
        s.vz = cfg.initial_vz_mps;
        s.q0 = 1.0; s.q1 = s.q2 = s.q3 = 0.0;
        s.p = s.q = s.r = 0.0;

        const double dt = 0.02;

        while (s.z > 0.0 && !tps_state.failed) {

            double V = std::sqrt(s.vx * s.vx + s.vz * s.vz);
            double Mach = V / speed_of_sound(s.z);
            double gamma = std::atan2(-s.vz, s.vx);

            double rho = density(s.z);
            double Rex = rho * V * veh.cref / 1.8e-5;

            HeatFlux H = compute_heating(
                rho,
                V,
                cfg.nose_radius_m,
                gamma,
                Mach,
                Rex,
                tps_state.T_surface,
                tps.emissivity
            );

            out.peak_q = std::max(out.peak_q, H.q_net);

            tps_state = update_tps_ablation(
                tps_state,
                tps,
                H.q_net,
                dt
            );

            out.peak_T_ratio = std::max(
                out.peak_T_ratio,
                tps_state.T_surface / tps.Tmax
            );

            out.remaining_tps = std::min(
                out.remaining_tps,
                tps_state.thickness / cfg.tps_thickness_m
            );

            GuidanceCmd gcmd =
                skip_guidance(s.z, Mach, H.q_net, gamma, cfg);

            s.vz += V * gcmd.gamma_dot_cmd * dt;
            s = rk4_step_6dof(s, dt, veh);
        }

        out.failure_mode = tps_state.failure_mode;
        return out;
    }

    // ======================================
    // Phase-3O — Explicit stage orchestrator
    // ======================================
    void run_default_simulation() {

        std::cout << "ORCUS Phase-3 Engine Running\n";

        // -------- Phase-3K --------
        print_stage_banner(OrcusStage::PHASE_3K);

        OrcusConfig cfg = default_config();
        ThermalSummary base = run_thermal_summary(cfg, 0.0);

        std::cout << "--- Phase-3K Baseline Thermal Margins ---\n";
        std::cout << "Peak T/Tmax           : " << base.peak_T_ratio << "\n";
        std::cout << "Remaining TPS fraction: " << base.remaining_tps << "\n";
        std::cout << "Peak heat flux        : " << base.peak_q << "\n";
        std::cout << "Failure mode          : "
            << static_cast<int>(base.failure_mode) << "\n";

        // -------- Phase-3N --------
        print_stage_banner(OrcusStage::PHASE_3N);
        run_worst_case_envelope();

        // -------- Phase-3P --------
        print_stage_banner(OrcusStage::PHASE_3P);

        // Uncertainty applied ON TOP of Phase-3K baseline
        run_uncertainty_analysis(
            density(30000.0),                 // representative high-alt density
            base.peak_q,                      // from Phase-3K
            base.peak_T_ratio * 2200.0,       // actual surface temp
            cfg.tps_thickness_m,
            2200.0
        );

        // -------- Phase-3O --------
        print_stage_banner(OrcusStage::PHASE_3O);
        std::cout << "All stages executed with certification traceability.\n";

        // -------- Phase-3Q --------
        print_stage_banner(OrcusStage::PHASE_3Q);
        run_minimum_tps_search();

		// -------- Phase-3W --------
        print_stage_banner(OrcusStage::PHASE_3W);
		run_montecarlo_certification();
        std::cout << "Failure mode       :"
        <<to_string(base.failure_mode) <<"\n";

    }

} // namespace ORCUS
