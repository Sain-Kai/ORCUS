#pragma once
#include "orcus_config.h"
#include "orcus_tps.h"
#include "orcus_dynamics.h"
#include <vector>

namespace ORCUS {

    struct TPSTimeHistory {
        std::vector<double> time;          // seconds
        std::vector<double> thickness;     // minimum TPS thickness (m)
        std::vector<double> heat_flux;     // peak heat flux (W/m^2)
        bool failed;
        double failure_time;
    };

    TPSTimeHistory run_tps_time_history(
        const OrcusConfig& cfg,
        const Vehicle6DOF& veh,
        const TPSMaterial& tps
    );

} // namespace ORCUS
