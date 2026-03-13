#ifndef TOY_PASSES
#define TOY_PASSES

include "mlir/Pass/PassBase.td"

def AffineFullUnroll : Pass<"affine-full-unroll"> {
    let summary = "Fully unroll all affine loops";
    let description = [{
        Fully unroll all affine loops.
    }];
    let dependentDialects = ["mlir::affine::AffineDialect"];
}

#endif
