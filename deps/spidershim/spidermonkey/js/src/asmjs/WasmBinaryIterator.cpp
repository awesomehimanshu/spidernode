/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=8 sts=4 et sw=4 tw=99:
 *
 * Copyright 2015 Mozilla Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "asmjs/WasmBinaryIterator.h"

using namespace js;
using namespace js::jit;
using namespace js::wasm;

#ifdef DEBUG
ExprKind
wasm::Classify(Expr expr)
{
    switch (expr) {
      case Expr::Block:
        return ExprKind::Block;
      case Expr::Loop:
        return ExprKind::Loop;
      case Expr::Unreachable:
        return ExprKind::Unreachable;
      case Expr::I32Const:
        return ExprKind::I32;
      case Expr::I64Const:
        return ExprKind::I64;
      case Expr::F32Const:
        return ExprKind::F32;
      case Expr::F64Const:
        return ExprKind::F64;
      case Expr::I8x16Const:
        return ExprKind::I8x16;
      case Expr::I16x8Const:
        return ExprKind::I16x8;
      case Expr::I32x4Const:
        return ExprKind::I32x4;
      case Expr::B8x16Const:
        return ExprKind::B8x16;
      case Expr::B16x8Const:
        return ExprKind::B16x8;
      case Expr::B32x4Const:
        return ExprKind::B32x4;
      case Expr::F32x4Const:
        return ExprKind::F32x4;
      case Expr::Br:
        return ExprKind::Br;
      case Expr::BrIf:
        return ExprKind::BrIf;
      case Expr::BrTable:
        return ExprKind::BrTable;
      case Expr::Nop:
        return ExprKind::Nullary;
      case Expr::I32Clz:
      case Expr::I32Ctz:
      case Expr::I32Popcnt:
      case Expr::I64Clz:
      case Expr::I64Ctz:
      case Expr::I64Popcnt:
      case Expr::F32Abs:
      case Expr::F32Neg:
      case Expr::F32Ceil:
      case Expr::F32Floor:
      case Expr::F32Trunc:
      case Expr::F32Nearest:
      case Expr::F32Sqrt:
      case Expr::F64Abs:
      case Expr::F64Neg:
      case Expr::F64Ceil:
      case Expr::F64Floor:
      case Expr::F64Trunc:
      case Expr::F64Nearest:
      case Expr::F64Sqrt:
      case Expr::I32BitNot:
      case Expr::I32Abs:
      case Expr::F64Sin:
      case Expr::F64Cos:
      case Expr::F64Tan:
      case Expr::F64Asin:
      case Expr::F64Acos:
      case Expr::F64Atan:
      case Expr::F64Exp:
      case Expr::F64Log:
      case Expr::I32Neg:
      case Expr::I8x16neg:
      case Expr::I8x16not:
      case Expr::I16x8neg:
      case Expr::I16x8not:
      case Expr::I32x4neg:
      case Expr::I32x4not:
      case Expr::F32x4neg:
      case Expr::F32x4sqrt:
      case Expr::F32x4abs:
      case Expr::F32x4reciprocalApproximation:
      case Expr::F32x4reciprocalSqrtApproximation:
      case Expr::B8x16not:
      case Expr::B16x8not:
      case Expr::B32x4not:
        return ExprKind::Unary;
      case Expr::I32Add:
      case Expr::I32Sub:
      case Expr::I32Mul:
      case Expr::I32DivS:
      case Expr::I32DivU:
      case Expr::I32RemS:
      case Expr::I32RemU:
      case Expr::I32And:
      case Expr::I32Or:
      case Expr::I32Xor:
      case Expr::I32Shl:
      case Expr::I32ShrS:
      case Expr::I32ShrU:
      case Expr::I32Rotl:
      case Expr::I32Rotr:
      case Expr::I64Add:
      case Expr::I64Sub:
      case Expr::I64Mul:
      case Expr::I64DivS:
      case Expr::I64DivU:
      case Expr::I64RemS:
      case Expr::I64RemU:
      case Expr::I64And:
      case Expr::I64Or:
      case Expr::I64Xor:
      case Expr::I64Shl:
      case Expr::I64ShrS:
      case Expr::I64ShrU:
      case Expr::I64Rotl:
      case Expr::I64Rotr:
      case Expr::F32Add:
      case Expr::F32Sub:
      case Expr::F32Mul:
      case Expr::F32Div:
      case Expr::F32Min:
      case Expr::F32Max:
      case Expr::F32CopySign:
      case Expr::F64Add:
      case Expr::F64Sub:
      case Expr::F64Mul:
      case Expr::F64Div:
      case Expr::F64Min:
      case Expr::F64Max:
      case Expr::F64CopySign:
      case Expr::I32Min:
      case Expr::I32Max:
      case Expr::F64Mod:
      case Expr::F64Pow:
      case Expr::F64Atan2:
      case Expr::I8x16add:
      case Expr::I8x16sub:
      case Expr::I8x16mul:
      case Expr::I8x16addSaturate:
      case Expr::I8x16subSaturate:
      case Expr::I8x16addSaturateU:
      case Expr::I8x16subSaturateU:
      case Expr::I8x16and:
      case Expr::I8x16or:
      case Expr::I8x16xor:
      case Expr::I16x8add:
      case Expr::I16x8sub:
      case Expr::I16x8mul:
      case Expr::I16x8addSaturate:
      case Expr::I16x8subSaturate:
      case Expr::I16x8addSaturateU:
      case Expr::I16x8subSaturateU:
      case Expr::I16x8and:
      case Expr::I16x8or:
      case Expr::I16x8xor:
      case Expr::I32x4add:
      case Expr::I32x4sub:
      case Expr::I32x4mul:
      case Expr::I32x4and:
      case Expr::I32x4or:
      case Expr::I32x4xor:
      case Expr::F32x4add:
      case Expr::F32x4sub:
      case Expr::F32x4mul:
      case Expr::F32x4div:
      case Expr::F32x4min:
      case Expr::F32x4max:
      case Expr::F32x4minNum:
      case Expr::F32x4maxNum:
      case Expr::B8x16and:
      case Expr::B8x16or:
      case Expr::B8x16xor:
      case Expr::B16x8and:
      case Expr::B16x8or:
      case Expr::B16x8xor:
      case Expr::B32x4and:
      case Expr::B32x4or:
      case Expr::B32x4xor:
        return ExprKind::Binary;
      case Expr::I32Eq:
      case Expr::I32Ne:
      case Expr::I32LtS:
      case Expr::I32LtU:
      case Expr::I32LeS:
      case Expr::I32LeU:
      case Expr::I32GtS:
      case Expr::I32GtU:
      case Expr::I32GeS:
      case Expr::I32GeU:
      case Expr::I64Eq:
      case Expr::I64Ne:
      case Expr::I64LtS:
      case Expr::I64LtU:
      case Expr::I64LeS:
      case Expr::I64LeU:
      case Expr::I64GtS:
      case Expr::I64GtU:
      case Expr::I64GeS:
      case Expr::I64GeU:
      case Expr::F32Eq:
      case Expr::F32Ne:
      case Expr::F32Lt:
      case Expr::F32Le:
      case Expr::F32Gt:
      case Expr::F32Ge:
      case Expr::F64Eq:
      case Expr::F64Ne:
      case Expr::F64Lt:
      case Expr::F64Le:
      case Expr::F64Gt:
      case Expr::F64Ge:
        return ExprKind::Comparison;
      case Expr::I32Eqz:
      case Expr::I32WrapI64:
      case Expr::I32TruncSF32:
      case Expr::I32TruncUF32:
      case Expr::I32ReinterpretF32:
      case Expr::I32TruncSF64:
      case Expr::I32TruncUF64:
      case Expr::I64ExtendSI32:
      case Expr::I64ExtendUI32:
      case Expr::I64TruncSF32:
      case Expr::I64TruncUF32:
      case Expr::I64TruncSF64:
      case Expr::I64TruncUF64:
      case Expr::I64ReinterpretF64:
      case Expr::I64Eqz:
      case Expr::F32ConvertSI32:
      case Expr::F32ConvertUI32:
      case Expr::F32ReinterpretI32:
      case Expr::F32ConvertSI64:
      case Expr::F32ConvertUI64:
      case Expr::F32DemoteF64:
      case Expr::F64ConvertSI32:
      case Expr::F64ConvertUI32:
      case Expr::F64ConvertSI64:
      case Expr::F64ConvertUI64:
      case Expr::F64ReinterpretI64:
      case Expr::F64PromoteF32:
      case Expr::I32x4fromFloat32x4:
      case Expr::I32x4fromFloat32x4U:
      case Expr::F32x4fromInt32x4:
      case Expr::F32x4fromUint32x4:
      case Expr::I32x4fromFloat32x4Bits:
      case Expr::I32x4fromInt8x16Bits:
      case Expr::I32x4fromInt16x8Bits:
      case Expr::I16x8fromInt8x16Bits:
      case Expr::I16x8fromInt32x4Bits:
      case Expr::I16x8fromFloat32x4Bits:
      case Expr::I8x16fromInt16x8Bits:
      case Expr::I8x16fromInt32x4Bits:
      case Expr::I8x16fromFloat32x4Bits:
      case Expr::F32x4fromInt8x16Bits:
      case Expr::F32x4fromInt16x8Bits:
      case Expr::F32x4fromInt32x4Bits:
        return ExprKind::Conversion;
      case Expr::I32Load8S:
      case Expr::I32Load8U:
      case Expr::I32Load16S:
      case Expr::I32Load16U:
      case Expr::I64Load8S:
      case Expr::I64Load8U:
      case Expr::I64Load16S:
      case Expr::I64Load16U:
      case Expr::I64Load32S:
      case Expr::I64Load32U:
      case Expr::I32Load:
      case Expr::I64Load:
      case Expr::F32Load:
      case Expr::F64Load:
      case Expr::I8x16load:
      case Expr::I16x8load:
      case Expr::I32x4load:
      case Expr::I32x4load1:
      case Expr::I32x4load2:
      case Expr::I32x4load3:
      case Expr::F32x4load:
      case Expr::F32x4load1:
      case Expr::F32x4load2:
      case Expr::F32x4load3:
        return ExprKind::Load;
      case Expr::I32Store8:
      case Expr::I32Store16:
      case Expr::I64Store8:
      case Expr::I64Store16:
      case Expr::I64Store32:
      case Expr::I32Store:
      case Expr::I64Store:
      case Expr::F32Store:
      case Expr::F64Store:
      case Expr::F32StoreF64:
      case Expr::F64StoreF32:
      case Expr::I8x16store:
      case Expr::I16x8store:
      case Expr::I32x4store:
      case Expr::I32x4store1:
      case Expr::I32x4store2:
      case Expr::I32x4store3:
      case Expr::F32x4store:
      case Expr::F32x4store1:
      case Expr::F32x4store2:
      case Expr::F32x4store3:
        return ExprKind::Store;
      case Expr::Select:
        return ExprKind::Select;
      case Expr::GetLocal:
      case Expr::LoadGlobal:
        return ExprKind::GetVar;
      case Expr::SetLocal:
      case Expr::StoreGlobal:
        return ExprKind::SetVar;
      case Expr::Call:
        return ExprKind::Call;
      case Expr::CallIndirect:
        return ExprKind::CallIndirect;
      case Expr::CallImport:
        return ExprKind::CallImport;
      case Expr::Return:
      case Expr::Limit:
        // Accept Limit, for use in decoding the end of a function after the body.
        return ExprKind::Return;
      case Expr::If:
        return ExprKind::If;
      case Expr::Else:
        return ExprKind::Else;
      case Expr::End:
        return ExprKind::End;
      case Expr::I32AtomicsLoad:
        return ExprKind::AtomicLoad;
      case Expr::I32AtomicsStore:
        return ExprKind::AtomicStore;
      case Expr::I32AtomicsBinOp:
        return ExprKind::AtomicBinOp;
      case Expr::I32AtomicsCompareExchange:
        return ExprKind::AtomicCompareExchange;
      case Expr::I32AtomicsExchange:
        return ExprKind::AtomicExchange;
      case Expr::I8x16extractLane:
      case Expr::I8x16extractLaneU:
      case Expr::I16x8extractLane:
      case Expr::I16x8extractLaneU:
      case Expr::I32x4extractLane:
      case Expr::F32x4extractLane:
      case Expr::B8x16extractLane:
      case Expr::B16x8extractLane:
      case Expr::B32x4extractLane:
        return ExprKind::ExtractLane;
      case Expr::I8x16replaceLane:
      case Expr::I16x8replaceLane:
      case Expr::I32x4replaceLane:
      case Expr::F32x4replaceLane:
      case Expr::B8x16replaceLane:
      case Expr::B16x8replaceLane:
      case Expr::B32x4replaceLane:
        return ExprKind::ReplaceLane;
      case Expr::I8x16swizzle:
      case Expr::I16x8swizzle:
      case Expr::I32x4swizzle:
      case Expr::F32x4swizzle:
        return ExprKind::Swizzle;
      case Expr::I8x16shuffle:
      case Expr::I16x8shuffle:
      case Expr::I32x4shuffle:
      case Expr::F32x4shuffle:
        return ExprKind::Shuffle;
      case Expr::I16x8check:
      case Expr::I16x8splat:
      case Expr::I32x4check:
      case Expr::I32x4splat:
      case Expr::I8x16check:
      case Expr::I8x16splat:
      case Expr::F32x4check:
      case Expr::F32x4splat:
      case Expr::B16x8check:
      case Expr::B16x8splat:
      case Expr::B32x4check:
      case Expr::B32x4splat:
      case Expr::B8x16check:
      case Expr::B8x16splat:
        return ExprKind::Splat;
      case Expr::I8x16select:
      case Expr::I16x8select:
      case Expr::I32x4select:
      case Expr::F32x4select:
        return ExprKind::SimdSelect;
      case Expr::I8x16Constructor:
      case Expr::I16x8Constructor:
      case Expr::I32x4Constructor:
      case Expr::F32x4Constructor:
      case Expr::B8x16Constructor:
      case Expr::B16x8Constructor:
      case Expr::B32x4Constructor:
        return ExprKind::SimdCtor;
      case Expr::B8x16allTrue:
      case Expr::B8x16anyTrue:
      case Expr::B16x8allTrue:
      case Expr::B16x8anyTrue:
      case Expr::B32x4allTrue:
      case Expr::B32x4anyTrue:
        return ExprKind::SimdBooleanReduction;
      case Expr::I8x16shiftLeftByScalar:
      case Expr::I8x16shiftRightByScalar:
      case Expr::I8x16shiftRightByScalarU:
      case Expr::I16x8shiftLeftByScalar:
      case Expr::I16x8shiftRightByScalar:
      case Expr::I16x8shiftRightByScalarU:
      case Expr::I32x4shiftLeftByScalar:
      case Expr::I32x4shiftRightByScalar:
      case Expr::I32x4shiftRightByScalarU:
        return ExprKind::SimdShiftByScalar;
      case Expr::I8x16equal:
      case Expr::I8x16notEqual:
      case Expr::I8x16greaterThan:
      case Expr::I8x16greaterThanOrEqual:
      case Expr::I8x16lessThan:
      case Expr::I8x16lessThanOrEqual:
      case Expr::I8x16greaterThanU:
      case Expr::I8x16greaterThanOrEqualU:
      case Expr::I8x16lessThanU:
      case Expr::I8x16lessThanOrEqualU:
      case Expr::I16x8equal:
      case Expr::I16x8notEqual:
      case Expr::I16x8greaterThan:
      case Expr::I16x8greaterThanOrEqual:
      case Expr::I16x8lessThan:
      case Expr::I16x8lessThanOrEqual:
      case Expr::I16x8greaterThanU:
      case Expr::I16x8greaterThanOrEqualU:
      case Expr::I16x8lessThanU:
      case Expr::I16x8lessThanOrEqualU:
      case Expr::I32x4equal:
      case Expr::I32x4notEqual:
      case Expr::I32x4greaterThan:
      case Expr::I32x4greaterThanOrEqual:
      case Expr::I32x4lessThan:
      case Expr::I32x4lessThanOrEqual:
      case Expr::I32x4greaterThanU:
      case Expr::I32x4greaterThanOrEqualU:
      case Expr::I32x4lessThanU:
      case Expr::I32x4lessThanOrEqualU:
      case Expr::F32x4equal:
      case Expr::F32x4notEqual:
      case Expr::F32x4greaterThan:
      case Expr::F32x4greaterThanOrEqual:
      case Expr::F32x4lessThan:
      case Expr::F32x4lessThanOrEqual:
        return ExprKind::SimdComparison;
      case Expr::CurrentMemory:
      case Expr::GrowMemory:
        break;
    }
    MOZ_MAKE_COMPILER_ASSUME_IS_UNREACHABLE("unimplemented opcode");
}
#endif
