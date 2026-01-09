#include "../include/orcus_surface_chemistry.h"
#include <algorithm>

namespace ORCUS {

    SurfaceChemistryProps compute_surface_chemistry(
        double T_wall,
        CatalysisModel model
    ) {
        SurfaceChemistryProps sc{};
        sc.model = model;

        // -----------------------------------------
        // Catalytic recombination efficiency
        // -----------------------------------------
        switch (model) {

        case CatalysisModel::NON_CATALYTIC:
            sc.efficiency = 0.0;
            break;

        case CatalysisModel::PARTIALLY_CATALYTIC:
            sc.efficiency = 0.3;
            break;

        case CatalysisModel::FULLY_CATALYTIC:
            sc.efficiency = 1.0;
            break;
        }

        // -----------------------------------------
        // Temperature dependence (recombination)
        // -----------------------------------------
        double temp_factor;

        if (T_wall < 800.0) {
            temp_factor = 0.3;
        }
        else if (T_wall < 1800.0) {
            temp_factor = 0.3 + 0.0005 * (T_wall - 800.0);
        }
        else {
            temp_factor = 0.8;
        }

        // -----------------------------------------
        // Heat augmentation due to recombination
        // -----------------------------------------
        sc.heat_multiplier =
            1.0 + sc.efficiency * temp_factor;

        sc.heat_multiplier =
            std::clamp(sc.heat_multiplier, 1.0, 1.8);

        return sc;
    }

}
