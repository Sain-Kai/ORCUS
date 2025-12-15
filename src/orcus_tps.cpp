#include "../include/orcus_tps.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    static constexpr double sigma = 5.670374419e-8;

    TPSState update_tps_ablation(
        const TPSState& s,
        const TPSMaterial& mat,
        double q_in,
        double dt
    ) {
        TPSState out = s;

        // -------------------------------
        // Phase-3O: hard stop if failed
        // -------------------------------
        if (out.failed)
            return out;

        if (out.thickness <= 0.0) {
            out.failed = true;
            out.failure_mode = TPSFailureMode::TPS_EXHAUSTED;
            return out;
        }

        // -------------------------------
        // Mass model
        // -------------------------------
        double A = 1.0; // unit area
        double ms = mat.density * out.thickness * A;
        double mb = ms * 2.0;

        // -------------------------------
        // Heat balance terms
        // -------------------------------
        double q_rad = mat.emissivity * sigma *
            std::pow(out.T_surface, 4.0);

        double q_cond = mat.k *
            (out.T_surface - out.T_bulk) /
            std::max(out.thickness, 1e-6);

        double q_net = q_in - q_rad - q_cond;

        // ==================================================
        // 🔥 ADDITIVE PHYSICS: Ablation Gas Shielding
        // ==================================================
        // As TPS erodes, pyrolysis gas reduces convective heat
        double shielding = 1.0;

        if (out.thickness < 0.8 * s.thickness)
            shielding = 0.70;   // mild shielding

        if (out.thickness < 0.5 * s.thickness)
            shielding = 0.55;   // strong shielding

        if (out.thickness < 0.3 * s.thickness)
            shielding = 0.40;   // heavy shielding

        q_net *= shielding;

        // -------------------------------
        // Ablation
        // -------------------------------
        double mdot = std::max(0.0, q_net / mat.L_abl);
        double dth = (mdot / mat.density) * dt;

        out.thickness -= dth;
        if (out.thickness <= 0.0) {
            out.thickness = 0.0;
            out.failed = true;
            out.failure_mode = TPSFailureMode::TPS_EXHAUSTED;
            return out;
        }

        // -------------------------------
        // Temperature evolution
        // -------------------------------
        out.T_surface += (q_net * dt) / (ms * mat.cp);
        out.T_bulk += (q_cond * dt) / (mb * mat.cp);

        // -------------------------------
        // Failure check
        // -------------------------------
        if (out.T_surface >= mat.Tmax) {
            out.failed = true;
            out.failure_mode = TPSFailureMode::OVER_TEMPERATURE;
        }

        return out;
    }

} // namespace ORCUS
