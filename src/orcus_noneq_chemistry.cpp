#include "../include/orcus_noneq_chemistry.h"
#include <algorithm>
#include <cmath>

namespace ORCUS {

    NoneqChemistryProps compute_noneq_chemistry(
        double T_stag,
        double Mach
    ) {
        NoneqChemistryProps nc{};

        // -----------------------------------------
        // Dissociation onset (oxygen + nitrogen)
        // -----------------------------------------
        if (T_stag < 2500.0) {
            nc.dissociation_fraction = 0.0;
        }
        else if (T_stag < 6000.0) {
            nc.dissociation_fraction =
                (T_stag - 2500.0) / (6000.0 - 2500.0);
        }
        else {
            nc.dissociation_fraction = 1.0;
        }

        nc.dissociation_fraction =
            std::clamp(nc.dissociation_fraction, 0.0, 1.0);

        // -----------------------------------------
        // Chemical relaxation (finite-rate effects)
        // Higher Mach → stronger nonequilibrium
        // -----------------------------------------
        nc.relaxation_factor =
            1.0 - std::exp(-Mach / 8.0);

        nc.relaxation_factor =
            std::clamp(nc.relaxation_factor, 0.0, 1.0);

        // -----------------------------------------
        // Net heat-flux correction
        // Dissociation absorbs energy → reduces heat
        // -----------------------------------------
        nc.heat_multiplier =
            1.0 - 0.35 *
            nc.dissociation_fraction *
            nc.relaxation_factor;

        nc.heat_multiplier =
            std::clamp(nc.heat_multiplier, 0.6, 1.0);

        return nc;
    }

}
