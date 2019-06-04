IR Layering
===========

GOTO will support `Multi-Layering IR` for a more modular, simplified code generation process.

Optimization Benefits
---------------------

The goal of GOTO is to make optimization passes easy to modify and extend. This means we need a more simplified IR than most currently used formats. The programming language will be described in the highest level IR. Lower, more specific IR layers, will be used for specific optimizations like `set[cond] instruction optimization`, `EH optimziation`, `cmov optimization`. We call this *IR Layering* and IR Layering will decrease dependencies of each optimizations stages.

IR Highering / Lowering
-----------------------

IR Lowering will be performed just as other compiler frameworks, except it will be lowered in each layer. `Layer 4` can only lower to `Layer 3`, and `Layer 3` can only lower to `Layer 2`. This will reduce the cross-layer dependencies and improve transform potential.

*High transform slots* will implement IR highering to implement features such as JIT crossgening from `x86` to `ARM64`. Highering is easier across IR nodes than from assembly to IR. For example the IR of `Layer 1` can be transformed to `Layer 2` more easily than transforming `Machine Assembly` to `IR`. This is often used to support JIT virtualization.
