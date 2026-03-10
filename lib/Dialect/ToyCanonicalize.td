#ifndef TOY_CANONICALIZE
#define TOY_CANONICALIZE

include "Dialect/ToyOps.td"
include "mlir/IR/PatternBase.td"

def BroadcastSplatPattern : Pat<
    (Toy_BroadcastOp (Toy_SplatOp $x)),
    (Toy_SplatOp $x)
>;

def BroadcastBroadcastPattern : Pat<
    (Toy_BroadcastOp (Toy_BroadcastOp $x)),
    (Toy_BroadcastOp $x)
>;

#endif
