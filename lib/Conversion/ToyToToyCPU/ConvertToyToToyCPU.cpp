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

struct ConvertToyToToyCPU : impl::ConvertToyToToyCPUBase<ConvertToyToToyCPU> {
    using ConvertToyToToyCPUBase::ConvertToyToToyCPUBase;

    void runOnOperation() override {
        MLIRContext *ctx = &getContext();
        auto *module = getOperation();

        ConversionTarget target(*ctx);
        target.addIllegalDialect<ToyDialect>();
        target.addLegalDialect<ArithDialect>();

        RewritePatternSet patterns(ctx);
        ConvertToyToToyCPUTypeConverter typeConverter(ctx);
        patterns.add<ConvertAddPtr>(typeConverter, ctx);

        populateFunctionOpInterfaceTypeConversionPattern<func::FuncOp>(patterns, typeConverter);
        target.addDynamicallyLegalOp<func::FuncOp>([&](func::FuncOp op) {
            return typeConverter.isSignatureLegal(op.getFunctionType()) &&
                   typeConverter.isLegal(&op.getBody());
        });

        populateReturnOpTypeConversionPattern(patterns, typeConverter);
        target.addDynamicallyLegalOp<func::ReturnOp>([&](func::ReturnOp op) {
            return typeConverter.isLegal(op);
        });

        if (failed(applyPartialConversion(module, target,
                                          std::move(patterns)))) {
            signalPassFailure();
        }
    }
};

}
}