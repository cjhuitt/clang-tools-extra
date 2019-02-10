set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidySmellsModule
  DataClumpCheck.cpp
  SmellsTidyModule.cpp

  LINK_LIBS
  clangAnalysis
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangSerialization
  clangTidy
  clangTidyUtils
  clangTooling
  )
