#pragma once

namespace ORCUS {

    enum class CatalysisModel {
        NON_CATALYTIC,
        PARTIALLY_CATALYTIC,
        FULLY_CATALYTIC
    };

    struct SurfaceChemistryProps {
        CatalysisModel model;
        double efficiency;     // 0–1 recombination efficiency
        double heat_multiplier; // Heat augmentation factor
    };

    // Computes surface chemistry effect on wall heating
    SurfaceChemistryProps compute_surface_chemistry(
        double T_wall,               // Wall temperature (K)
        CatalysisModel model          // Wall catalysis model
    );

}
