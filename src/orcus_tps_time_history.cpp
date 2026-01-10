#include "../include/orcus_tps_time_history.h"
#include "../include/orcus_dynamics.h"
#include "../include/orcus_physics.h"
#include "../include/orcus_heat.h"
#include "../include/orcus_guidance.h"
#include "../include/orcus_tps.h"

#include <cmath>
#include <algorithm>

namespace ORCUS {

    TPSTimeHistory run_tps_time_history(
        const OrcusConfig& cfg,
        const Vehicle6DOF& veh,
        const TPSMaterial& tps
    ) {
        TPSTimeHistory hist{};
        hist.failed = false;
        hist.failure_time = -1.0;

        // -------------------------------
        // Initial vehicle state
        // -------------------------------
        State6DOF s{};
        s.z = cfg.initial_altitude_m;
        s.vx = cfg.initial_speed_mps;
        s.vz = cfg.initial_vz_mps;
        s.q0 = 1.0;

        TPSState tps_state{};
        tps_state.T_surface = 300.0;
        tps_state.T_bulk = 300.0;
        tps_state.thickness = cfg.tps_thickness_m;
        tps_state.failed = false;
        tps_state.failure_mode = TPSFailureMode::NONE;

        double t = 0.0;
        double dt = 0.02;

        // -------------------------------
        // Time marching
        // -------------------------------
        while (s.z > 0.0 && !tps_state.failed && t < 2000.0) {

            double V = std::sqrt(s.vx * s.vx + s.vz * s.vz);
            double Mach = V / speed_of_sound(s.z);
            double gamma = std::atan2(-s.vz, s.vx);

            double rho = density(s.z);
            double Rex = rho * V * veh.cref / 1.8e-5;

            HeatFlux H = compute_heating(
                rho, V, cfg.nose_radius_m,
                gamma, Mach, Rex,
                tps_state.T_surface,
                tps.emissivity
            );

            // Guidance
            GuidanceCmd gcmd =
                skip_guidance(s.z, Mach, H.q_net, gamma, cfg);

            s.vz += V * gcmd.gamma_dot_cmd * dt;

            // TPS update
            tps_state =
                update_tps_ablation(tps_state, tps, H.q_net, dt);

            // Record history
            hist.time.push_back(t);
            hist.thickness.push_back(tps_state.thickness);
            hist.heat_flux.push_back(H.q_net);

            if (tps_state.failed && !hist.failed) {
                hist.failed = true;
                hist.failure_time = t;
            }

            // Integrate dynamics
            s = rk4_step_6dof(s, dt, veh);

            t += dt;
        }

        return hist;
    }

} // namespace ORCUS
