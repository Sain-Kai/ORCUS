📦 CHANGELOG — ORCUS

All significant technical changes are recorded here.

[v0.4-A] — Aerothermal Physical Closure (Current)
Added

Enthalpy-limited convective heating model

Shock-temperature-based radiative heating

Physically closed heat balance

Coupled surface–bulk TPS temperature model

Explicit conductive heat transport

Distinct TPS failure modes

Changed

Removed velocity³-only heating dependence

Replaced non-physical heat spikes

Aligned heating, TPS, and atmosphere time-scales

Corrected unit consistency across thermal stack

Fixed

Infinite / exploding heat flux artifacts

False survivability due to missing radiation

TPS failure ambiguity

[v0.3] — Certification Stack & Failure Analysis
Added

Phase-3K thermal margin quantification

Phase-3N worst-case envelope search

Phase-3P uncertainty robustness analysis

Phase-3Q minimum TPS closure

Phase-3W Monte-Carlo certification

Changed

Introduced staged phase orchestration

Centralized failure reporting

Unified thermal summary reporting

[v0.2] — Aerodynamics & Heating Foundation
Added

Atmospheric density & temperature models

Mach-based regime handling

Initial stagnation heating logic

Dynamic pressure tracking

Changed

Modularized physics layers

Improved RK4 numerical stability

[v0.1] — Core Re-Entry Skeleton
Added

3-DOF equations of motion

RK4 integrator

Basic project structure

Initial trajectory propagation

Planned (Not Implemented Yet)

CFD / wind-tunnel validated heating tables

Reynolds-number transition modeling

Real-gas chemistry effects

Guidance & control optimization

Visualization & telemetry export