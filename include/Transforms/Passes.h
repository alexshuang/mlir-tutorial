#ifndef TUTORIAL_TRANSFORMS_PASSES_H_
#define TUTORIAL_TRANSFORMS_PASSES_H_

#include "Transforms/AffineFullUnroll.h"

namespace mlir {
namespace toy {

#define GEN_PASS_REGISTRATION
#include "Transforms/Passes.h.inc"

}
}

#endif
