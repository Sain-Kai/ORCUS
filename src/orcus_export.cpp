#include "../include/orcus_export.h"
#include <fstream>
#include <iomanip>

namespace ORCUS {

    void export_surface_csv(
        const std::string& filename,
        const std::vector<ExportSurfacePoint>& data
    ) {
        std::ofstream f(filename);
        if (!f.is_open()) return;

        f << "x,q_wall,tps_remaining,T_structure,thermal_stress\n";

        for (const auto& p : data) {
            f << std::fixed << std::setprecision(6)
                << p.x << ","
                << p.q_wall << ","
                << p.tps_remaining << ","
                << p.T_structure << ","
                << p.thermal_stress << "\n";
        }

        f.close();
    }

}
