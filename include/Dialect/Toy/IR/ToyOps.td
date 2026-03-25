#ifndef TOY_OPS
#define TOY_OPS

include "Dialect/Toy/IR/ToyDialect.td"
include "Dialect/Toy/IR/ToyTypes.td"
include "mlir/IR/OpBase.td"
include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/Bytecode/BytecodeOpInterface.td"

class Toy_Op<string mnemonic, list<Trait> traits = []> : Op<Toy_Dialect, mnemonic, traits> {}

def Toy_AddPtrOp : Toy_Op<"addptr", [Pure, ElementwiseMappable,
                                     SameOperandsAndResultShape,
                                     TypesMatchWith<"Result type match with ptr type",
                                                    "res", "ptr", "$_self">]> {
    let summary = "Add pointer addresss with an offset";
    let description = [{
        Add pointer addresss with an offset.
    }];

    let arguments = (ins Toy_PtrLike:$ptr, Toy_IntLike:$offset);
    let results = (outs Toy_PtrLike:$res);

    let assemblyFormat = "$ptr `,` $offset attr-dict `:` type($res) `,` type($offset)";
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
    let arguments = (ins Toy_ArrayAttr:$value);
    let results = (outs Toy_PtrTensor:$output);
    let assemblyFormat = "$value attr-dict `:` qualified(type($output))";
    let hasFolder = 1;
}

def Toy_SplatOp : Toy_Op<"splat", [Pure, SameOperandsAndResultElementType]> {
    let summary = "Splat offset(int) to tensor of offset";
    let description = [{
        Splat offset(int) to tensor of offset
    }];
    let arguments = (ins Toy_Type:$input);
    let results = (outs Toy_Tensor:$output);
    let assemblyFormat = "$input attr-dict `:` type($input) `->` type($output)";
    let hasFolder = 1;
}

def Toy_BroadcastOp : Toy_Op<"broadcast", [Pure,
                                           SameOperandsAndResultElementType]> {
    let summary = "Broadcast a tensor";
    let description = [{
        Broadcast one or more dims with size 1 to a new size,
        e.g. tensor<1x3xf32> -> tensor<3x3xf32>.
    }];
    let arguments = (ins Toy_Tensor:$src);
    let results = (outs Toy_Tensor:$out);
    let assemblyFormat = "$src attr-dict `:` type($src) `->` type($out)";
    let hasCanonicalizer = 1;
}

#endif
