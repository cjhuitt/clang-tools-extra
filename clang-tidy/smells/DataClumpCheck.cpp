//===--- DataClumpCheck.cpp - clang-tidy ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DataClumpCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace smells {

void DataClumpCheck::registerMatchers(MatchFinder *Finder) {
    auto paramaterizedFunction = functionDecl();
    Finder->addMatcher(
        paramaterizedFunction.bind("checkParamsForClumpsToMethodArgs"), this);
}

void DataClumpCheck::check(const MatchFinder::MatchResult &Result) {
    if (const auto func =
        Result.Nodes.getNodeAs<FunctionDecl>("checkParamsForClumpsToMethodArgs"))
    {
        if (!func->doesThisDeclarationHaveABody()) return;
        if (func->getNumParams() < 2) return;

        struct Match
        {
            std::set<ParmVarDecl*> params;
            const CallExpr* call;
        };
        std::vector<Match> matches;

        SourceLocation function_loc;
        function_loc = func->getLocation();
        auto params = func->parameters();

//        diag(function_loc, "Checking " +
//            func->getQualifiedNameAsString() + " at B");

        for ( const auto call : findCalls(func->getBody()) )
        {
            if ( call->getNumArgs() < 2 ) continue;

            Match match;
            match.call = call;

//            diag(call->getBeginLoc(), "Checking function " +
//                call->getDirectCallee()->getQualifiedNameAsString());
            for ( const auto arg : findNamedArgs( call ) )
            {
                const auto decl = arg->getFoundDecl();
                const auto name = decl->getQualifiedNameAsString();
//                diag(arg->getBeginLoc(), "Checking argument " + name);

                auto pred = [name]( const ParmVarDecl* param ) -> bool {
                    return name == param->getQualifiedNameAsString();
                };
                auto found = std::find_if( params.begin(), params.end(), pred );
                if ( found != params.end() )
                {
                    match.params.insert( *found );
                }
            }

            if ( match.params.size() < 2 ) continue;

            matches.emplace_back( match );
        }

        if ( matches.empty() ) return;

        auto join = []( const std::vector<std::string>& strings ) -> std::string {
            if ( strings.size() == 2 )
            {
                return strings[0] + " and " + strings[1];
            }

            std::string joined;
            auto iter = strings.begin();
            auto last = strings.end() - 1;
            for ( ; iter != last; ++iter )
            {
                joined += *iter;
                joined += ", ";
            }
            joined += "and " + *last;
            return joined;
        };

        diag(function_loc, "Data clump detected in " +
            func->getQualifiedNameAsString());
        for ( const auto& match : matches )
        {
            std::vector<std::string> param_names;
            for ( const auto param : params )
            {
                if ( match.params.find( param ) != match.params.end() )
                {
                    param_names.emplace_back( param->getQualifiedNameAsString() );
                }
            }
            auto parms = join( param_names );
            diag(match.call->getBeginLoc(), parms + " passed directly to " +
                match.call->getDirectCallee()->getQualifiedNameAsString());
        }
    }
}

std::vector<const CallExpr*> DataClumpCheck::findCalls(const Stmt *statement)
{
    std::vector<const CallExpr*> calls;
    for ( const auto child : statement->children() )
    {
        if( isa<CallExpr>( child ) )
        {
            calls.emplace_back( cast<CallExpr>(child) );
        }
        else
        {
            auto subcalls = findCalls( child );
            calls.insert( calls.end(), subcalls.begin(), subcalls.end());
        }
    }

    return calls;
}

std::vector<const DeclRefExpr*> DataClumpCheck::findNamedArgs(const CallExpr *call)
{
    std::vector<const DeclRefExpr*> args;
    for ( const auto arg : call->arguments() )
    {
        const auto fixed = arg->IgnoreImplicit();
        if( isa<DeclRefExpr>( fixed ) )
        {
            args.emplace_back( cast<DeclRefExpr>( fixed ) );
        }
    }
    return args;
}

} // namespace smells
} // namespace tidy
} // namespace clang
