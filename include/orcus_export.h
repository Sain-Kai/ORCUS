#pragma once
#include <vector>
#include <string>

namespace ORCUS {

    struct ExportSurfacePoint {
        double x;
        double q_wall;
        double tps_remaining;
        double T_structure;
        double thermal_stress;
    };

    void export_surface_csv(
        const std::string& filename,
        const std::vector<ExportSurfacePoint>& data
    );

}
