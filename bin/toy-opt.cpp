#include "mlir/Tools/mlir-opt/MlirOptMain.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/Affine/IR/AffineOps.h"

#include "Transforms/Passes.h"
#include "Dialect/ToyDialect.h"

void registerToyDialects(mlir::DialectRegistry &registry) {
    registry.insert<mlir::arith::ArithDialect, mlir::func::FuncDialect,
                    mlir::affine::AffineDialect, mlir::toy::ToyDialect
                    >();
}

int main(int argc, char **argv) {
    mlir::DialectRegistry registry;
    registerToyDialects(registry);

    mlir::toy::registerToyPasses();

    return mlir::asMainReturnCode(
        mlir::MlirOptMain(argc, argv, "Tutorial pass driver", registry)
    );
}