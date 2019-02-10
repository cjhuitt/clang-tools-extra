//===--- DataClumpCheck.h - clang-tidy --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SMELLS_DATACLUMPCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SMELLS_DATACLUMPCHECK_H

#include "../ClangTidy.h"

namespace clang {
namespace tidy {
namespace smells {

/// Detects data clumps inside functions
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/smells-data-clump.html
class DataClumpCheck : public ClangTidyCheck {
public:
  DataClumpCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
private:
  std::vector<const CallExpr*> findCalls(const Stmt *statement);
  std::vector<const DeclRefExpr*> findNamedArgs(const CallExpr *call);
};

} // namespace smells
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SMELLS_DATACLUMPCHECK_H
