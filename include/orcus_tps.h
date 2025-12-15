#pragma once

namespace ORCUS {

    enum class TPSFailureMode {
        NONE = 0,

        TPS_EXHAUSTED = 1,
        OVER_TEMPERATURE = 2,

        ABLATION = TPS_EXHAUSTED,
        OVER_TEMP = OVER_TEMPERATURE
    };

    struct TPSState {
        double T_surface;
        double T_bulk;
        double thickness;
        bool failed;
        TPSFailureMode failure_mode;
    };

    struct TPSMaterial {
        double density;
        double cp;
        double k;
        double emissivity;
        double Tmax;
        double L_abl;
    };

    TPSState update_tps_ablation(
        const TPSState& s,
        const TPSMaterial& mat,
        double q_in,
        double dt
    );

    
    inline const char* to_string(TPSFailureMode m) {
        switch (m) {
        case TPSFailureMode::NONE: return "NONE";
        case TPSFailureMode::TPS_EXHAUSTED: return "TPS_EXHAUSTED";
        case TPSFailureMode::OVER_TEMPERATURE: return "OVER_TEMPERATURE";
        default: return "UNKNOWN";
        }
    }

} // namespace ORCUS
