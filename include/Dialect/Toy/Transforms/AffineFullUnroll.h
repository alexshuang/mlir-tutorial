#ifndef TUTORIAL_TRANSFORMS_AFFINEFULLUNROLL_H_
#define TUTORIAL_TRANSFORMS_AFFINEFULLUNROLL_H_

#include "mlir/Pass/Pass.h"

namespace mlir {
namespace toy {

#define GEN_PASS_DECL_AFFINEFULLUNROLL
#include "Dialect/Toy/Transforms/Passes.h.inc"

}
}

#endif
