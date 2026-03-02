#ifndef TOY_OPS
#define TOY_OPS

include "ToyDialect.td"
include "ToyTypes.td"
include "mlir/IR/OpBase.td"
include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/Bytecode/BytecodeOpInterface.td"

class Toy_Op<string mnemonic, list<Trait> traits = []> : Op<Toy_Dialect, mnemonic, traits> {}

def Toy_AddPtrOp : Toy_Op<"addptr", [Pure, ElementwiseMappable]> {
    let summary = "Add pointer addresss with an offset";
    let description = [{
        Add pointer addresss with an offset.
    }];

    let arguments = (ins Toy_PtrLike:$ptr, Toy_IntLike:$offset);
    let results = (outs Toy_PtrLike:$res);

    let assemblyFormat = "$ptr `,` $offset attr-dict `:` type($ptr) `,` type($offset) `->` type($res)";
    let hasFolder = 1;
}

def Toy_FromTensorOp : Toy_Op<"from_tensor", [Pure]> {
    let summary = "Create ptrTensor from tensor";
    let description = [{
        Create ptrTensor from tensor
    }];
    let arguments = (ins TensorOf<[AnyInteger]>:$input);
    let results = (outs TensorOf<[Toy_Ptr]>:$output);
    let assemblyFormat = "$input attr-dict `:` type($input) `->` type($output)";
    let hasFolder = 1;
}

def Toy_ConstantOp : Toy_Op<"constant", [Pure, ConstantLike]> {
    let summary = "The creator of toy tensor/vector/scalar";
    let description = [{
        The creator of toy tensor/vector/scalar
    }];
    let arguments = (ins DenseI64ArrayAttr:$coeff);
    let results = (outs Toy_PtrTensor:$output);
    let assemblyFormat = "$coeff attr-dict `:` qualified(type($output))";
    let hasFolder = 1;
}

#endif
