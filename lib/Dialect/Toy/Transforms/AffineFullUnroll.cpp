#include "mlir/Dialect/Affine/IR/AffineOps.h"
#include "mlir/Dialect/Affine/LoopUtils.h"
#include "mlir/Pass/Pass.h"
#include "Dialect/Toy/Transforms/AffineFullUnroll.h"

namespace mlir {
namespace toy {

using mlir::affine::AffineForOp;
using mlir::affine::loopUnrollFull;

#define GEN_PASS_DEF_AFFINEFULLUNROLL
#include "Dialect/Toy/Transforms/Passes.h.inc"

struct AffineFullUnroll : public impl::AffineFullUnrollBase<AffineFullUnroll> {
    using AffineFullUnrollBase::AffineFullUnrollBase;

    void runOnOperation() {
        auto mod = getOperation();
        mod->walk([&](AffineForOp op){
            if (failed(loopUnrollFull(op))) {
                op.emitError("unrolling failed");
                signalPassFailure();
            }
        });
    }
};

}
}