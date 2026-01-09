#include "../include/orcus_displacement_bl.h"

namespace ORCUS {

    DisplacementBL compute_displacement_thickness(
        double theta,
        double Cf,
        bool turbulent
    ) {
        DisplacementBL d{};

        // Shape factor H = delta*/theta
        // Typical hypersonic values
        double H;

        if (turbulent) {
            H = 1.4;   // turbulent BL
        }
        else {
            H = 2.6;   // laminar BL
        }

        d.shape_factor = H;
        d.delta_star = H * theta;

        return d;
    }

}
