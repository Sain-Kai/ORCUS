🛰️ ORCUS — Hypersonic Re-Entry Vehicle Simulator
Summary

ORCUS is a modular, physics-driven hypersonic atmospheric re-entry vehicle simulator focused on aerothermodynamic fidelity, TPS survivability, and certification-style robustness analysis.

Unlike visualization-centric tools, ORCUS is designed as a failure-aware engineering simulator, where why and how a vehicle fails is as important as whether it survives.

Core Objective (Non-Negotiable)

Maximize physical realism for hypersonic re-entry within a transparent, extensible research codebase.

Every phase added so far exists to:

Reduce hidden assumptions

Expose physical sensitivities

Prevent “false survivability”

Prepare the model for CFD / wind-tunnel anchoring

What ORCUS Can Do Today (v0.4-A)
1. Flight Dynamics

3-DOF point-mass re-entry equations

Gravity-turn trajectory propagation

Bank-angle controlled lift modulation

Numerically stable RK4 integrator

Explicit vehicle state evolution (position, velocity, attitude rates)

2. Atmosphere & Gas Properties

Layered atmosphere model

Tropospheric lapse rate

Exponential upper-atmosphere decay

Altitude-varying:

Density

Temperature

Speed of sound

Gravity

Mach number computed from local thermodynamics (not constants)

3. Aerothermal Heating (Phase-3 → Phase-4A)

Enthalpy-limited stagnation heating

Fay–Riddell–style formulation

Based on total stagnation enthalpy (not velocity³ alone)

Radiative heating

Activated in hypersonic regime (Mach > 10)

Density- and shock-temperature-dependent

Surface re-radiation

Stefan–Boltzmann emission

Net heat balance explicitly closed:

q_net = q_conv + q_rad − q_emit


⚠️ This eliminated earlier non-physical heat spikes and infinite flux artifacts.

4. Thermal Protection System (TPS) Physics

Material-resolved TPS model:

Density

Specific heat

Conductivity

Emissivity

Ablation energy

Maximum allowable temperature

Coupled surface + bulk temperature evolution

Explicit ablation mass loss

Conductive heat transport through TPS thickness

Physically meaningful failure modes:

TPS_EXHAUSTED

OVER_TEMPERATURE

No artificial “survival flags” — failure emerges naturally.

5. Phase-3 Certification Stack (Completed)

Each phase exists for engineering validation, not decoration.

Phase-3K — Thermal Margin Quantification

Baseline trajectory evaluation

Peak heat flux

Peak temperature ratio

Remaining TPS margin

Phase-3N — Worst-Case Envelope

Sweeps over:

Entry flight-path angle

Bank angle

Nose radius

Identifies true worst-case re-entry

Detects hidden non-survivable corners

Phase-3P — Uncertainty Robustness

Conservative stacking of:

Atmospheric density uncertainty

Aerodynamic uncertainty

Heating model uncertainty

Survival assessed under compounded worst-case assumptions

Phase-3Q — Minimum TPS Closure

Iterative TPS thickness testing

Answers:

“What is the minimum TPS that survives all certified phases?”

Phase-3W — Monte-Carlo Certification

Hundreds to thousands of randomized runs

Tracks:

Survival rate

Failure statistics

Worst-case outcomes

Produces a certification matrix, not a single trajectory lie

6. Failure-First Design Philosophy

ORCUS does not tune parameters to “make it survive”.

If the model fails:

The physics is interrogated

The assumption is exposed

The fix is justified

This is intentional and correct for hypersonic research.

Scientific Assumptions (Explicit & Traceable)

ORCUS currently assumes:

Continuum flow regime

Frozen chemistry (no dissociation / ionization yet)

Stagnation-point heating dominance

Axisymmetric blunt body

No aero-thermal shape change

Open-loop guidance (no feedback control)

Each assumption is isolated in code, making removal or upgrade feasible.

Architecture Overview
ORCUS/
│
├── include/
│   ├── orcus_core.h          # Phase orchestration
│   ├── orcus_physics.h       # Atmosphere & gas properties
│   ├── orcus_heat.h          # Aerothermal models
│   ├── orcus_tps.h           # TPS physics & failure
│   ├── orcus_guidance.h      # Skip guidance logic
│   ├── orcus_envelope.h      # Worst-case sweeps
│   └── orcus_constants.h
│
├── src/
│   ├── core/
│   ├── physics/
│   ├── thermal/
│   ├── dynamics/
│   └── certification/
│
├── data/
├── README.md
└── CHANGELOG.md

Versioning Policy

0.x → Physics closure & certification logic

1.0 → CFD / wind-tunnel anchored heating

2.0 → Guidance, control & optimization

Build Instructions
Windows (Visual Studio)

Open ORCUS.sln

Build: x64 | Debug / Release

Run executable

GCC / MinGW
g++ -std=c++17 -O2 src/**/*.cpp -Iinclude -o orcus
./orcus

License

MIT License — see LICENSE.md

Author

Sohan Manna
B.Tech (VIT)
Hypersonic Flight • Aerothermodynamics • Failure-Aware Simulation

ORCUS is built to fail honestly before it ever claims to survive.