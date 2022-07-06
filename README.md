# Raygen

Raygen is a header-only library binding [raylib](https://github.com/raysan5/raylib) and Eigen3.

Aimming at rapid prototyping of interactive 3D scenes. Raygen is basically a toolset for the purpose of my Computer Graphics Study.

## Examples

**Ex1. Orbit Camera Control**

`Alt+Left Mouse Button` to drag and rotate, `Mouse Wheel` to zoom in/out.

<img src="cover_img/example1.png" alt="ex1" width="300"/>

**Ex2. Parallel For Loop Utilization**
```c++
  for (int i = 0; i < size_long; i++) {
    // some unparalleled computation
  }

  Raygen::parallel_for(size_long, [&](const int i) {
    // some paralleled computation
  });
```

```
[         Unparalleled Computing :        754.25ms      ]
[           Paralleled Computing :        64.973ms      ]
```

**Ex3. Mass Spring Game**

Click to add nodes to this mass string system.

<img src="cover_img/example3.png" alt="ex3" width="300"/>

**Ex4. Position Based Fluids**

[TODO]
