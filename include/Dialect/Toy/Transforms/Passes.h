#ifndef TUTORIAL_TRANSFORMS_PASSES_H_
#define TUTORIAL_TRANSFORMS_PASSES_H_

#include "Dialect/Toy/Transforms/AffineFullUnroll.h"

namespace mlir {
namespace toy {

#define GEN_PASS_REGISTRATION
#include "Dialect/Toy/Transforms/Passes.h.inc"

}
}

#endif
