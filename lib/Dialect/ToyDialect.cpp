#include "Dialect/ToyDialect.h"
#include "Dialect/ToyDialect.cpp.inc"
#include "llvm/ADT/TypeSwitch.h"
#include "mlir/IR/Builders.h"
#include "llvm/Support/Debug.h"

#define GET_TYPEDEF_CLASSES
#include "Dialect/ToyTypes.cpp.inc"

#define GET_OP_CLASSES
#include "Dialect/ToyOps.cpp.inc"

namespace mlir {
namespace toy {

void ToyDialect::initialize() {
    // Used to register types and operations with the dialect
    addTypes<
#define GET_TYPEDEF_LIST
#include "Dialect/ToyTypes.cpp.inc"
    >();

    addOperations<
#define GET_OP_LIST
#include "Dialect/ToyOps.cpp.inc"
    >();
}

::mlir::OpFoldResult ConstantOp::fold(FoldAdaptor adaptor) {
    return adaptor.getCoeffAttr();
}

::mlir::OpFoldResult FromTensorOp::fold(FoldAdaptor adaptor) {
    auto input = dyn_cast_or_null<DenseI64ArrayAttr>(adaptor.getInput());
    if (!input)
        return {};
    return input;
}

::mlir::OpFoldResult AddPtrOp::fold(FoldAdaptor adaptor) {
    auto ptr = dyn_cast_or_null<DenseI64ArrayAttr>(adaptor.getPtr());
    auto offset = dyn_cast_or_null<IntegerAttr>(adaptor.getOffset());
    if (!ptr || !offset)
        return {};

    int64_t off = offset.getValue().getSExtValue();
    SmallVector<int64_t> res;
    res.reserve(ptr.size());
    for (int64_t p: ptr.asArrayRef())
        res.push_back(p + off);

    return DenseI64ArrayAttr::get(getContext(), res);
}

::mlir::Operation *ToyDialect::materializeConstant(::mlir::OpBuilder &builder,
                                                   ::mlir::Attribute value,
                                                   ::mlir::Type type,
                                                   ::mlir::Location loc) {
    auto coeff = dyn_cast<DenseI64ArrayAttr>(value);
    if (!coeff)
        return nullptr;
    return builder.create<ConstantOp>(loc, type, coeff);
}

}
}
