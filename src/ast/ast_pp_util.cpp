/*++
Copyright (c) 2015 Microsoft Corporation

Module Name:

    ast_pp_util.cpp

Abstract:

    <abstract>

Author:

    Nikolaj Bjorner (nbjorner) 2015-8-6.

Revision History:

--*/

#include "ast/ast_pp_util.h"
#include "ast/ast_smt2_pp.h"
#include "ast/ast_smt_pp.h"

void ast_pp_util::collect(expr* e) {
    coll.visit(e);
}

void ast_pp_util::collect(unsigned n, expr* const* es) {
    for (unsigned i = 0; i < n; ++i) {
        coll.visit(es[i]);
    }
}

void ast_pp_util::collect(expr_ref_vector const& es) {
    collect(es.size(), es.c_ptr());
}

void ast_pp_util::display_decls(std::ostream& out) {
    ast_smt_pp pp(m);
    unsigned n = coll.get_num_sorts();
    for (unsigned i = 0; i < n; ++i) {
        pp.display_ast_smt2(out, coll.get_sorts()[i], 0, 0, 0);
    }
    n = coll.get_num_decls();
    for (unsigned i = 0; i < n; ++i) {
        func_decl* f = coll.get_func_decls()[i];
        if (f->get_family_id() == null_family_id) {
            ast_smt2_pp(out, f, m_env) << "\n";
        }
    }
    SASSERT(coll.get_num_preds() == 0);
}

void ast_pp_util::display_asserts(std::ostream& out, expr_ref_vector const& fmls, bool neat) {
    if (neat) {
        for (expr* f : fmls) {
            out << "(assert ";
            ast_smt2_pp(out, f, m_env);
            out << ")\n";
        }
    }
    else {
        ast_smt_pp ll_smt2_pp(m);
        for (expr* f : fmls) {
            out << "(assert ";
            ll_smt2_pp.display_expr_smt2(out, f);
            out << ")\n";
        }
    }
}
