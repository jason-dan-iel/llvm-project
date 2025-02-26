//===------ SemaAVR.cpp ---------- AVR target-specific routines -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file implements semantic analysis functions specific to AVR.
//
//===----------------------------------------------------------------------===//

#include "clang/Sema/SemaAVR.h"
#include "clang/AST/DeclBase.h"
#include "clang/Basic/DiagnosticSema.h"
#include "clang/Sema/Attr.h"
#include "clang/Sema/ParsedAttr.h"
#include "clang/Sema/Sema.h"

namespace clang {
SemaAVR::SemaAVR(Sema &S) : SemaBase(S) {}

void SemaAVR::handleInterruptAttr(Decl *D, const ParsedAttr &AL) {
  if (!isFuncOrMethodForAttrSubject(D)) {
    Diag(D->getLocation(), diag::warn_attribute_wrong_decl_type)
        << AL << AL.isRegularKeywordAttribute() << ExpectedFunction;
    return;
  }

  if (!AL.checkExactlyNumArgs(SemaRef, 0))
    return;

  // AVR interrupt handlers must have no parameter and be void type.
  if (hasFunctionProto(D) && getFunctionOrMethodNumParams(D) != 0) {
    Diag(D->getLocation(), diag::warn_interrupt_signal_attribute_invalid)
        << /*AVR*/ 3 << /*interrupt*/ 0 << 0;
    return;
  }
  if (!getFunctionOrMethodResultType(D)->isVoidType()) {
    Diag(D->getLocation(), diag::warn_interrupt_signal_attribute_invalid)
        << /*AVR*/ 3 << /*interrupt*/ 0 << 1;
    return;
  }

  handleSimpleAttribute<AVRInterruptAttr>(*this, D, AL);
}

void SemaAVR::handleSignalAttr(Decl *D, const ParsedAttr &AL) {
  if (!isFuncOrMethodForAttrSubject(D)) {
    Diag(D->getLocation(), diag::warn_attribute_wrong_decl_type)
        << AL << AL.isRegularKeywordAttribute() << ExpectedFunction;
    return;
  }

  if (!AL.checkExactlyNumArgs(SemaRef, 0))
    return;

  // AVR signal handlers must have no parameter and be void type.
  if (hasFunctionProto(D) && getFunctionOrMethodNumParams(D) != 0) {
    Diag(D->getLocation(), diag::warn_interrupt_signal_attribute_invalid)
        << /*AVR*/ 3 << /*signal*/ 1 << 0;
    return;
  }
  if (!getFunctionOrMethodResultType(D)->isVoidType()) {
    Diag(D->getLocation(), diag::warn_interrupt_signal_attribute_invalid)
        << /*AVR*/ 3 << /*signal*/ 1 << 1;
    return;
  }

  handleSimpleAttribute<AVRSignalAttr>(*this, D, AL);
}

} // namespace clang
