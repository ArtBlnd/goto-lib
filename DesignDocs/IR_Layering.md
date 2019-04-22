IR Layering
===========

Feature GOTO library will support `Multi-Layering IR` to make more simplified, specific kind of process on each layer.

Optimization Benefits
---------------------

In optimziation benefits. for example, some kind of `Branch Elimination Optimization` or `Inline Optimization` can always useful on most of cpu architectures(like x86_64, ARM, etcs). So the goal is make general and easy optimization, which needs in more simplified IR code will be performed on highests level of IR layer. in another word, more specific IR layer, will perform more specific optimization, which something like `set[cond] instruction optimization`, `EH optimziation`. so IR Layering will decrease dependencies of each optimizations.

IR Highering / Lowering
-----------------------

The IR Lowering will perform same as another compiler frameworks, except it will lowered in each layer. `Layer 4` can only lower to `Layer 3`, and `Layer 3` can only lower to `Layer 2` to make less dependencies and high transform potential. so high transform slots will make IR to highering, which means specific assembly of IR nodes which is `Layer 1` can transform into `Layer 2` more easily than just transforming `Machine Assembly` into `IR` to support JIT virtualization. for example this will makes JIT crossgenning on x86 to ARM64.