//===--- SmellTidyModule.cpp - clang-tidy ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "DataClumpCheck.h"

namespace clang {
namespace tidy {
namespace smells {

class SmellsModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<DataClumpCheck>(
        "smells-data-clump");
  }
};

} // namespace smells

// Register the SmellTidyModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<smells::SmellsModule>
    X("smells-module", "Adds code smell lint checks.");

// This anchor is used to force the linker to link in the generated object file
// and thus register the SmellsModule.
volatile int SmellsModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang
