#include "Dialect/Toy/IR/Dialect.h"
#include "Dialect/Toy/IR/ToyDialect.cpp.inc"
#include "llvm/ADT/TypeSwitch.h"
#include "mlir/IR/Builders.h"
#include "llvm/Support/Debug.h"
#include "mlir/IR/BuiltinTypeInterfaces.h"
#include "mlir/IR/PatternMatch.h"

#define GET_TYPEDEF_CLASSES
#include "Dialect/Toy/IR/ToyTypes.cpp.inc"

#define GET_OP_CLASSES
#include "Dialect/Toy/IR/ToyOps.cpp.inc"

#include "Dialect/Toy/IR/ToyCanonicalize.cpp.inc"

namespace mlir {
namespace toy {

void ToyDialect::initialize() {
    // Used to register types and operations with the dialect
    addTypes<
#define GET_TYPEDEF_LIST
#include "Dialect/Toy/IR/ToyTypes.cpp.inc"
    >();

    addOperations<
#define GET_OP_LIST
#include "Dialect/Toy/IR/ToyOps.cpp.inc"
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
    if (!ptr)
        return {};

    SmallVector<int64_t> res;
    res.reserve(ptr.size());

    if (auto attr = dyn_cast_or_null<IntegerAttr>(adaptor.getOffset())) {
        auto off = attr.getValue().getSExtValue();
        for (auto o : ptr.asArrayRef())
            res.push_back(o + off);
    } else if (auto attr = dyn_cast_or_null<DenseElementsAttr>(adaptor.getOffset())) {
        if (!attr.getType().hasStaticShape() || attr.getNumElements() != ptr.size())
            return {};

        auto pa = ptr.asArrayRef();
        auto oa = attr.getValues<APInt>();
        for (auto it: llvm::zip(pa, oa))
            res.push_back(std::get<0>(it) + std::get<1>(it).getSExtValue());
    }

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

::mlir::OpFoldResult SplatOp::fold(FoldAdaptor adaptor) {
    auto value = adaptor.getInput();
    if (!value || !isa<FloatAttr, IntegerAttr>(value))
        return {};
    auto type = dyn_cast_or_null<ShapedType>(getType());
    if (!type)
        return {};
    return SplatElementsAttr::get(type, ArrayRef<Attribute>(value));
}

void BroadcastOp::getCanonicalizationPatterns(::mlir::RewritePatternSet &results,
                                              ::mlir::MLIRContext *context) {
    results.add<BroadcastSplatPattern, BroadcastBroadcastPattern>(context);
}

}
}
