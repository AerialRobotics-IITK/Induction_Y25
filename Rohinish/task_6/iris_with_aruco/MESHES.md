# Mesh dependency

`model.sdf` references visuals via
`model://iris_with_standoffs/meshes/iris.dae` (and the matching propeller
meshes / `iris_collision.stl`).  Those binary assets ship with the
**`ardupilot_gz`** package (and PX4's `gazebo-iris`), so we deliberately do
**not** duplicate them in this repository — Gazebo Harmonic resolves them
from the `GZ_SIM_RESOURCE_PATH` already set up by the simulator install.

If you ever want a fully self-contained model, copy the four files

* `iris.dae`
* `iris_collision.stl`
* `iris_prop_ccw.dae`
* `iris_prop_cw.dae`

into the `meshes/` folder and update the `uri` lines in `model.sdf`
accordingly.
