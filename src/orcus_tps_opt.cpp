#include "../include/orcus_tps_opt.h"
#include "../include/orcus_constants.h"
#include "../include/orcus_heat.h"
#include "../include/orcus_physics.h"
#include "../include/orcus_tps.h"
#include "../include/orcus_dynamics.h"
#include "../include/orcus_guidance.h"
#include "../include/orcus_config.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace ORCUS {

    void run_tps_optimization() {

        std::cout << "\n====================================\n";
        std::cout << "ORCUS Phase-3L — TPS TRADE STUDY\n";
        std::cout << "====================================\n";

        OrcusConfig cfg = default_config();

        constexpr double nose_radii[] = { 0.5, 0.75, 1.0, 1.25 };
        constexpr double thicknesses[] = { 0.08, 0.10, 0.12, 0.15, 0.18 };

        Vehicle6DOF veh{};
        veh.mass = 1500.0;
        veh.Sref = 1.8;
        veh.cref = 2.0;
        veh.bank = 0.35;
        veh.Ixx = 800;
        veh.Iyy = 1200;
        veh.Izz = 1000;

        TPSMaterial tps{ 1600.0,1250.0,0.15,0.9,2200.0,2.5e6 };

        std::vector<TPSOptResult> results;

        for (double Rn : nose_radii) {
            for (double thickness : thicknesses) {

                State6DOF s{};
                s.z = cfg.initial_altitude_m;
                s.vx = cfg.initial_speed_mps;
                s.vz = cfg.initial_vz_mps;
                s.q0 = 1.0;

                TPSState tps_state{};
                tps_state.T_surface = 300.0;
                tps_state.T_bulk = 300.0;
                tps_state.thickness = thickness;
                tps_state.failed = false;
                tps_state.failure_mode = TPSFailureMode::NONE;

                const double dt = 0.02;
                double peak_T = 0.0;

                while (s.z > 0.0 && !tps_state.failed) {

                    double V = std::sqrt(s.vx * s.vx + s.vz * s.vz);
                    double Mach = V / speed_of_sound(s.z);
                    double gamma = std::atan2(-s.vz, s.vx);

                    double rho = density(s.z);
                    double Rex = rho * V * veh.cref / 1.8e-5;

                    HeatFlux H = compute_heating(
                        rho, V, Rn,
                        gamma, Mach, Rex,
                        tps_state.T_surface,
                        tps.emissivity
                    );

                    GuidanceCmd gcmd =
                        skip_guidance(s.z, Mach, H.q_net, gamma, cfg);

                    s.vz += V * gcmd.gamma_dot_cmd * dt;

                    tps_state =
                        update_tps_ablation(tps_state, tps, H.q_net, dt);

                    peak_T = std::max(peak_T, tps_state.T_surface);

                    s = rk4_step_6dof(s, dt, veh);
                }

                TPSOptResult r{};
                r.nose_radius = Rn;
                r.thickness = thickness;
                r.survived = !tps_state.failed;
                r.peak_T = peak_T;
                r.ablated = thickness - tps_state.thickness;
                r.remaining = tps_state.thickness;

                results.push_back(r);

                std::cout << "3L | Rn=" << Rn
                    << " TPS=" << thickness
                    << " survived=" << r.survived << "\n";
            }
        }

        std::ofstream out("../../data/tps_trade_study.csv");
        out << "nose_radius,thickness,survived,peak_T,ablated,remaining\n";

        for (const auto& r : results) {
            out << r.nose_radius << ","
                << r.thickness << ","
                << r.survived << ","
                << r.peak_T << ","
                << r.ablated << ","
                << r.remaining << "\n";
        }

        std::cout << "ORCUS Phase-3L completed\n";
    }

}
