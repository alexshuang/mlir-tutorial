#include "Conversion/ToyToToyCPU/Passes.h"
#include "mlir/Transforms/DialectConversion.h"
#include "mlir/Dialect/Func/Transforms/FuncConversions.h"

namespace mlir {
namespace toy {

#define GEN_PASS_DEF_CONVERTTOYTOTOYCPU
#include "Conversion/ToyToToyCPU/Passes.h.inc"

using namespace mlir::toy;
using namespace mlir::arith;

class ConvertToyToToyCPUTypeConverter : public TypeConverter {
public:
    ConvertToyToToyCPUTypeConverter(MLIRContext *ctx) {
        addConversion([](Type type) { return type; });
        addConversion([ctx](PtrType type) -> Type {
            return type.getElemType();
        });
        addConversion([ctx](RankedTensorType tensorTy) -> Type {
            auto elemTy = tensorTy.getElementType();
            if (auto ptrTy = dyn_cast<PtrType>(elemTy)) {
                auto elemTy = ptrTy.getElemType();
                return RankedTensorType::get(tensorTy.getShape(), elemTy);
            }
            return tensorTy;
        });

        addSourceMaterialization([](OpBuilder &builder, Type type,
                                    ValueRange inputs, Location loc) {
            return builder.create<FromTensorOp>(loc, type, inputs[0]);
        });
        addTargetMaterialization([](OpBuilder &builder, Type type,
                                    ValueRange inputs, Location loc) {
            return builder.create<ToTensorOp>(loc, type, inputs[0]);
        });
    }
};

struct ConvertAddPtr : public OpConversionPattern<AddPtrOp> {
    using OpConversionPattern<AddPtrOp>::OpConversionPattern;

    LogicalResult matchAndRewrite(AddPtrOp op, OpAdaptor adaptor,
                                  ConversionPatternRewriter &rewriter) const override {
        auto loc = op.getLoc();
        auto res = rewriter.create<arith::AddIOp>(loc, adaptor.getPtr(),
                                                  adaptor.getOffset());
        rewriter.replaceOp(op, res.getResult());
        return success();
    }
};

struct ConvertConstant : public OpConversionPattern<ConstantOp> {
    using OpConversionPattern<ConstantOp>::OpConversionPattern;

    LogicalResult matchAndRewrite(ConstantOp op, OpAdaptor adaptor,
                                  ConversionPatternRewriter &rewriter) const override {
        auto loc = op.getLoc();
        auto arr = adaptor.getValue();
        auto first_elemTy = cast<TypedAttr>(arr[0]).getType();
        auto elemTy = cast<PtrType>(op.getType().getElementType()).getElemType();
        auto resTy = RankedTensorType::get({static_cast<int64_t>(arr.size())},
                                           elemTy);

        SmallVector<Attribute> elems;
        elems.reserve(arr.size());
        if (first_elemTy.isIntOrIndex()) {
            for (auto &o : arr) {
                auto attr = cast<IntegerAttr>(o);
                auto v = attr.getValue().sextOrTrunc(
                            elemTy.getIntOrFloatBitWidth());
                elems.push_back(IntegerAttr::get(elemTy, v));
            }
        } else {
            assert(isa<FloatType>(first_elemTy));
            auto &sem = cast<FloatType>(elemTy).getFloatSemantics();
            bool losesInfo = false;
            for (auto &o : arr) {
                APFloat v = cast<FloatAttr>(o).getValue();
                v.convert(sem, APFloat::rmNearestTiesToEven, &losesInfo);
                elems.push_back(FloatAttr::get(elemTy, v));
            }
        }

        auto denseAttr = DenseElementsAttr::get(resTy, elems);

        auto res = rewriter.create<arith::ConstantOp>(loc, resTy, denseAttr);
        rewriter.replaceOp(op, res.getResult());
        return success();
    }
};

struct ConvertToyToToyCPU : impl::ConvertToyToToyCPUBase<ConvertToyToToyCPU> {
    using ConvertToyToToyCPUBase::ConvertToyToToyCPUBase;

    void runOnOperation() override {
        MLIRContext *ctx = &getContext();
        auto *module = getOperation();

        ConversionTarget target(*ctx);
        target.addIllegalOp<AddPtrOp, ConstantOp>();
        target.addLegalDialect<ArithDialect>();

        RewritePatternSet patterns(ctx);
        ConvertToyToToyCPUTypeConverter typeConverter(ctx);
        patterns.add<ConvertAddPtr>(typeConverter, ctx);
        patterns.add<ConvertConstant>(typeConverter, ctx);

        // populateFunctionOpInterfaceTypeConversionPattern<func::FuncOp>(patterns, typeConverter);
        // target.addDynamicallyLegalOp<func::FuncOp>([&](func::FuncOp op) {
        //     return typeConverter.isSignatureLegal(op.getFunctionType()) &&
        //            typeConverter.isLegal(&op.getBody());
        // });

        // populateReturnOpTypeConversionPattern(patterns, typeConverter);
        // target.addDynamicallyLegalOp<func::ReturnOp>([&](func::ReturnOp op) {
        //     return typeConverter.isLegal(op);
        // });

        if (failed(applyPartialConversion(module, target,
                                          std::move(patterns)))) {
            signalPassFailure();
        }
    }
};

}
}