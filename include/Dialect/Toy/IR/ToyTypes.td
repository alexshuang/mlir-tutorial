#ifndef TOY_TYPES
#define TOY_TYPES

include "Dialect/Toy/IR/ToyDialect.td"
include "mlir/IR/AttrTypeBase.td"

class Toy_Type<string name, string typeMnemonic> : TypeDef<Toy_Dialect, name> {
    let mnemonic = typeMnemonic;
}

def Toy_Ptr : Toy_Type<"Ptr", "ptr"> {
    let summary = "Toy buffer pointer type";
    let description = [{
        Toy buffer pointer type
    }];

    let parameters = (ins "Type":$elemType);
    let assemblyFormat = "`<` $elemType `>`";
}
def Toy_PtrTensor : RankedTensorOf<[Toy_Ptr]>;
def Toy_PtrLike : AnyTypeOf<[Toy_Ptr, Toy_PtrTensor]>;

def Toy_Float : AnyTypeOf<[F16, F32, F64], "Float">;
def Toy_FloatTensor : RankedTensorOf<[Toy_Float]>;
def Toy_FloatLike : AnyTypeOf<[Toy_Float, Toy_FloatTensor]>;

def Toy_Int : AnyTypeOf<[I1, I8, I16, I32, I64], "Integer">;
def Toy_IntTensor : RankedTensorOf<[Toy_Int]>;
def Toy_IntLike : AnyTypeOf<[Toy_Int, Toy_IntTensor]>;

def Toy_IntOrFloat : AnyTypeOf<[Toy_Int, Toy_Float]>;

def Toy_Tensor : AnyTypeOf<[Toy_PtrTensor, Toy_IntTensor, Toy_FloatTensor]>;
def Toy_Type : AnyTypeOf<[Toy_PtrLike, Toy_FloatLike, Toy_IntLike]>;

def Toy_Attr : AnyAttrOf<[I64Attr, I32Attr, I8Attr, BoolAttr, F64Attr,
                          F32Attr]>;

def Toy_ArrayAttr : TypedArrayAttrBase<Toy_Attr, "array of toy attributes">;

#endif
