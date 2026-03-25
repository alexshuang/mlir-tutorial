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
    auto ptrArr = dyn_cast_or_null<ArrayAttr>(adaptor.getPtr());
    if (!ptrArr)
        return {};

    SmallVector<Attribute> res;
    res.reserve(ptrArr.size());

    if (auto attr = dyn_cast_or_null<IntegerAttr>(adaptor.getOffset())) {
        auto off = attr.getValue().getSExtValue();
        for (auto o : ptrArr) {
            auto iAttr = dyn_cast_or_null<IntegerAttr>(o);
            if (!iAttr)
                return {};

            auto ty = iAttr.getType();
            auto a = iAttr.getValue().getSExtValue();
            res.push_back(IntegerAttr::get(ty, a + off));
        }
    } else if (auto attr = dyn_cast_or_null<DenseElementsAttr>(adaptor.getOffset())) {
        if (!attr.getType().hasStaticShape() || attr.getNumElements() != ptrArr.size())
            return {};

        if (attr.getElementType().isIntOrIndex()) {
            auto val = attr.getValues<APInt>();
            auto it = val.begin();
            for (auto &o : ptrArr) {
                auto iAttr = dyn_cast_or_null<IntegerAttr>(o);
                if (!iAttr)
                    return {};

                auto ty = iAttr.getType();
                auto a = iAttr.getValue().getSExtValue();
                auto b = (*it).getSExtValue();
                res.push_back(IntegerAttr::get(ty, a + b));
                it++;
            }
        } else if (attr.getElementType().isF32() || attr.getElementType().isF64()) {
            auto val = attr.getValues<APFloat>();
            auto it = val.begin();
            for (auto &o : ptrArr) {
                auto fAttr = dyn_cast_or_null<FloatAttr>(o);
                if (!fAttr)
                    return {};

                auto ty = fAttr.getType();
                auto sum = fAttr.getValue();
                auto b = *it++;
                sum.add(b, APFloat::rmNearestTiesToEven);

                res.push_back(FloatAttr::get(ty, sum));
            }
        } else {
            return {};
        }
    }

    return ArrayAttr::get(getContext(), res);
}

::mlir::Operation *ToyDialect::materializeConstant(::mlir::OpBuilder &builder,
                                                   ::mlir::Attribute value,
                                                   ::mlir::Type type,
                                                   ::mlir::Location loc) {
    auto coeff = dyn_cast<ArrayAttr>(value);
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
