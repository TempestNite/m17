#include "parser.h"
#include <state.h>
#include <stdio.h>
#include <Declaration.h>
#include <DeclarationList.h>
#include <TypeInfo.h>
#include <semantic.h>
#include <symbol_table.h>
#include <Result.h>

Token* token;
Token* curTok;

int setupParser(int fd)
{
    printf("STEP1\n");
    setupScanner(fd);
    printf("SETUPSCAN\n");
    token = malloc(sizeof (*token) );
    token -> lineNumber = 1;
    startcic();
    getToken(token);
    curTok = malloc(sizeof (*token) );
    memcpy(curTok, token, sizeof(*token));
    memset(token, 0, sizeof(*token));
    startcic();
    getToken(token);
    return 0;
}

int program ()
{
    decldefn_list();
    clearToken(token);
    clearToken(curTok);
    free(token);
    free(curTok);
    return 0;

}

int decldefn_list()
{
    printf("DECLDEFN LIST ID: %d\n", token->id);
    if (decldefn() == 0)
        return 0;
    else if (decldefn_suf() == 0)
        return 1;
    else return 1;
}

int decldefn()
{
    printf("DECLDEFN\n");

    if (recognize(VOID_M) == 0)
    {
        // printf("IN\n");
        if (scalar_type() == 0)
        {
            if (recognize(STRUCT_M) == 0)
                return 0;
            else if (recognize(ID_M) == 0)
                return syntax_error("ID after struct\n");
            else if (stsuffix() == 0)
                return syntax_error("stsuffix after ID\n");
            else return 1;
        }
        else if (opt_modifier_list() == 0)
            return syntax_error("opt_modifier_list should be optional\n");
        else if (function() == 0)
            return syntax_error("function after opt_modifier_list\n");
        else return 1;
    }
    else if (function() == 0)
        return syntax_error("Function after void\n");
    else return 1;
}
// FIX
int decldefn_suf()
{
    printf("DECLDEFN_SUF\n");

    if (decldefn() == 0)
        return 0; // decldefn decldefsuf
    else if (decldefn_suf() == 0)
        return 1; // empty
    else return 1;
}

int scalar_type()
{
    printf("SCALAR_TYPE\n");
    if ( recognize(CHAR_M) || recognize(INT_M) || recognize(FLOAT_M) )
        return 1;
    else return 0;
}

int function()
{
    printf("FUNCTION\n");
    if ( recognize(ID_M) )
    {
        DeclarationList* fn_decl_list;
        Declaration decl;
        fn_decl_list = add_decl(0, &decl);
        if (recognize(lparenth) == 0)
            return syntax_error("Left paren after function identifier\n");
        else if (opt_parm_list() == 0)
            return syntax_error("Parm list should be optional\n");
        else if (recognize(rparenth) == 0)
            return syntax_error("Right paren after function identifier\n");
        else if (fcnsuf() == 0)
            return 1;
        else return 1;
    }
    else return 0;
}

int opt_parm_list()
{
    printf("OPT_PARM_LIST\n");
    if (parm_list() == 0)
        return 1;
    else return opt_parm_list();
}

int parm_list()
{
    printf("PARM_LIST\n");
    if (parm() == 0)
        return 0;
    else if (ropl() == 0)
        return 1;
    else return 1;
}

int parm()
{
    printf("PARM\n");

    if (type() == 0)
        return 0;
    else if (identifier_list() == 0)
        return syntax_error("identifier_list after type for parm\n");
    else return 1;
}

int ropl()
{
    printf("REST OF PARM LIST\n");
    printf("GG %s\n", curTok->lexeme);

    if (recognize(semicolon) == 0)
        return 0;
    else if (parm() == 0)
    {

        return syntax_error("parm after semicolon\n");
    }
    else if (ropl() == 0)
        return 1;
    else return 1;
}

int identifier_list()
{
    printf("IDENTIFIER LIST\n");
    if (recognize(ID_M) == 0)
        return 0;
    else if (roidl() == 0)
        return 1;
    else return 1; // FIX
}

int roidl()
{
    printf("ROIDL\n");
    if (recognize(comma) == 0)
        return 1;
    else if (recognize(ID_M) == 0)
        return syntax_error("no identifier following comma in roidl\n");
    else return roidl();
}

int type()
{
    printf("TYPE\n");
    if (simple_type() == 0)
        return 0;
    else if (opt_modifier_list() == 0)
        return syntax_error("MAY NEED TO CHANGE OPT MOD LIST\n");
    else return 1;
}

int expr_suf()
{
    printf("EXPR_SUF\n");
    if (recognize(equal) == 0)
        return 0;
    else if (expr() == 0)
        return 1;
    else if (expr_suf() == 0)
        return 1;
    else return 1;
}

int opt_expr()
{
    printf("OPT_EXPR\n");
    if (expr() == 0)
        return 1;
    else return opt_expr();
}

int expr ()
{
    printf("EXPR\n");

    if (log_OR_expr() == 0)
        return 0;
    else if (expr_suf() == 0)
        return 1;
    else return 1;
}

int while_stmt()
{
    if (recognize(WHILE_M) == 0)
        return 0;
    else if (expr() == 0)
        return syntax_error("Expression after left paren\n");
    else if (recognize(lparenth) == 0)
        return syntax_error("Left paren after while missing\n");
    else if (stmt() == 0)
        return syntax_error("Statement after expression\n");
    else return 1;
}

int do_while_stmt()
{
    if (recognize(DO_M) == 0)
        return 0;
    else if (stmt() == 0)
        return syntax_error("Stmt after do\n");
    else if (recognize(lparenth) == 0)
        return syntax_error("Left paren in do while\n");
    else if (expr() == 0)
        return syntax_error("expressions in do while\n");
    else if (recognize(rparenth) == 0)
        return syntax_error("Right paren in do while\n");
    else if (recognize(semicolon) == 0)
        return syntax_error("Semicolon at end of do while\n");
    else return 1;
}

int recognize(int tokenid)
{
    if (curTok->id == tokenid)
    {
        printf("MATCHED! %30s%10d\n", curTok->lexeme, curTok->id);
        nextTok(); 
        return 1;
    }
    else
    { 
        // fprintf(stderr, "ID not matched! %d vs tok: %d\n", tokenid, curTok->id);
        return 0;
    }
}

int nextTok()
{
    curTok = realloc(curTok, sizeof(*token));
    memcpy(curTok, token, sizeof(*token));
    memset(token, 0x0, sizeof(*token));
    startcic();
    getToken(token);
}

int opt_modifier_list()
{
    printf("OPT_MOD_LIST\n");
    if (modifier_list() == 0)
        return 1;
    else return opt_modifier_list();
}
 
int modifier_list()
{
    printf("MOD_LIST\n");
    if (modifier() == 0)
        return 0;
    else if (mlsuf() == 0)
        return 1;
    else return 1;
}

int modifier()
{
    printf("MODIFIER\n");
    if (recognize(lsbrace) == 0)
    {
        if (recognize(ast) == 0)
        {
            if (recognize(lparenth) == 0)
                return 0;
            else if (recognize(rparenth) == 0)
                return syntax_error("Rparen after lparen\n");
            else if (recognize(lparenth) == 0)
                return syntax_error("Lparen after paren pair in modifier\n");
            else if (opt_parm_list() == 0)
                return 1; // opt_parm_list() to always return 1
            else if (recognize(rparenth) == 0)
                return syntax_error("Rparen after lparen after (opt_parm_list)\n");
            else return 1;
        }
        else return 1;
    }
    else if (recognize(deci) == 0)
        return syntax_error("Decimal after left straight brace\n");
    else if (recognize(rsbrace) == 0)
        return syntax_error("Right brace after decimal for modifier");
    else return 1;
}

int declaration_stmt()
{
    printf("DECLSTMT\n");
    if (type() == 0)
        return 0;
    else if (identifier_list() == 0)
        return syntax_error("ID list after type\n");
    else if (recognize(semicolon) == 0)
        return syntax_error("Semicolon after identifier_list in declstmt\n");
    else return 1;
}

int opt_declarations()
{
    printf("OPTDECL\n");
    if (declaration_list() == 0)
        return 1;
    else return opt_declarations();
}

int declaration_list()
{
    printf("DECLLIST\n");
    if (declaration_stmt() == 0)
        return 0;
    else if (declsuf() == 0)
        return 1;
    else return 1; // FIX
}

int declsuf()
{
    printf("DECLSUF\n");
    if (declaration_stmt() == 0)
        return 0;
    else if (declsuf() == 0)
        return 1;
    else return 1;
}

int stmt_list()
{
    printf("STMTLIST\n");
    if (stmt() == 0)
        return 0;
    else if (rosl() == 0)
        return 1;
    else return 1;
}

int rosl()
{
    printf("ROSL\n");
    if (stmt() == 0)
        return 0;
    else if (rosl() == 0)
        return 1;
    else return 1;
}

int stmt()
{
    printf("STMT\n");
    if ( expr_stmt() || block_stmt() || while_stmt() || do_while_stmt() || \
            for_stmt() || if_stmt() || return_stmt() || declaration_stmt() )
        return 1;
    else return 0;
}

int expr_stmt()
{
    printf("EXPRSTMT\n");
    if (expr() == 0)
        return 0;
    else if (recognize(semicolon) == 0)
        return syntax_error("semicolon after expr\n");
    else return 1;
}

int block_stmt()
{
    printf("BLOCK_STATEMENT\n");
    if (recognize(lcbrace) == 0)
        return 0;

    else if (stmt_list() == 0)
        return 1;
    else if (recognize(rcbrace) == 0)
        return syntax_error("Right curly brace in block statement\n");
    else return 1;
}

int for_stmt()
{
    printf("FORSTMT\n");
    if (recognize(FOR_M) == 0)
        return 0;
    else if (recognize(lparenth) == 0)
        return syntax_error("Left paren after for stmt\n");
    else if (opt_expr() == 0)
        return 1;
    else if (recognize(semicolon) == 0)
        return syntax_error("semicolon after expression\n");
    else if (opt_expr() == 0)
        return 1;
    else if (recognize(semicolon) == 0)
        return syntax_error("semicolon after expression\n");
    else if (opt_expr() == 0)
        return 1;
    else if (recognize(rparenth) == 0)
        return syntax_error("Right paren after for stmt\n");
    else if (stmt() == 0)
        return syntax_error("Statement after for stmt\n");
    else return 1;
}

int if_stmt()
{
    printf("IFSTMT\n");
    if (recognize(IF_M) == 0)
        return 0;
    else if (recognize(lparenth) == 0)
        return syntax_error("Left paren after for stmt\n");
    else if (expr() == 0)
        return 1;
    else if (recognize(rparenth) == 0)
        return syntax_error("Right paren after for stmt\n");
    else if (stmt() == 0)
        return syntax_error("Statement after for stmt\n");
    else if (opt_else() == 0)
        return 1;
    else return 1;
}

int opt_else()
{
    printf("OPTELSESTMT\n");
    if (recognize(ELSE_M) == 0)
        return 1;
    else if (stmt() == 0)
        return syntax_error("stmt after else");
    else return opt_else();
}

int return_stmt()
{
    printf("RETURN STMT\n");
    if (recognize(RETURN_M) == 0)
        return 0;
    else if (opt_expr() == 0)
        return 1;
    else if (recognize(semicolon) == 0)
        return syntax_error("semicolon after return");
    else return 1;
}

int mlsuf()
{
    printf("MLSUF\n");
    if (modifier() == 0)
        return 0;
    else if (mlsuf() == 0)
        return 1;
    else return mlsuf();
}

int stsuffix()
{
    printf("STSUFFIX\n");
    if (recognize(lcbrace) == 0)
    {
        if (opt_modifier_list() == 0)
            return 0;
        else if (function() == 0)
            return syntax_error("Function after opt_modifier_list\n");
        else return 1;
    }
    else if (member_list() == 0)
        return syntax_error("Member list after left curly brace in stsuffix\n");
    else if (recognize(rcbrace) == 0)
        return syntax_error("Right curly brace after member list in stsuffix\n");
    else return 1;
}

int member_list()
{
    printf("MEMBERLIST\n");
    if (member() == 0)
        return 0;
    else if (roml() == 0)
        return 1; // FIX SIMPLIFY
    else return 1;
}

int roml()
{
    printf("ROML\n");
    if (member() == 0)
        return 0;
    else if (roml() == 0)
        return 1;
    else return 1;
}

int member()
{
    printf("MEMBER\n");
    if (type() == 0)
        return 0;
    else if (identifier_list() == 0)
        return syntax_error("identifier_list after type for member\n");
    else if (recognize(semicolon) == 0)
        return syntax_error("semicolon after identifier_list in member\n");
    else return 1;
}


int fcnsuf()
{
    printf("FCNSUF\n");
    if ( (recognize(semicolon) == 0) && (block_stmt() == 0) )
        return 0;
    else return 1;
}

int simple_type()
{
    printf("SIMPLE TYPE\n");
    if (scalar_type() == 0)
    {
        if (recognize(STRUCT_M) == 0)
            return 0;
        else if (recognize(ID_M) == 0)
            return syntax_error("ID_M after struct in simple_type");
        else return 1;
    }
    else return 1;
}

int log_OR_expr ()
{
    printf("log_OR_expr\n");
    if (log_AND_expr() == 0)
        return 0;
    else if (loe_suf() == 0)
        return 1;
    else return 1;
}

int loe_suf()
{
    printf("loe_suf\n");
    if (recognize(or) == 0)
        return 0;
    else if (log_OR_expr() == 0)
        return syntax_error("log_OR_expr after || operator\n");
    else if (loe_suf() == 0)
        return 1;
    else return 1;
}

int log_AND_expr()
{
    printf("log_AND_expr\n");
    if (relation_expr() == 0)
        return 0;
    else if (lae_suf() == 0)
        return 1;
    else return 1;
}

int lae_suf()
{
    printf("lae_suf\n");
    if (recognize(and) == 0)
        return 0;
    else if (relation_expr() == 0)
        return syntax_error("relation_expr after && comparison operator\n");
    else if (lae_suf() == 0)
        return 1;
    else return 1;
}

int relation_expr()
{
    printf("relation_expr\n");
    if (additive_expr() == 0)
        return 0;
    else if (rel_suf() == 0)
        return 1;
    else return 1;
}

int rel_suf()
{
    printf("rel_suf\n");
    if ( (recognize(equal) == 0) && (recognize(notequal) == 0) && (recognize(gt) == 0) &&
            (recognize(lt) == 0) && (recognize(le) == 0) && (recognize(ge) == 0) )
        return 0;
    else if (additive_expr() == 0)
        return syntax_error("additive_expr after == operator");
    else if(rel_suf() == 0)
        return 1;
    else return 1;
}

int additive_expr()
{
    printf("additive_expr\n");
    if (multiplicative_expr() == 0)
        return 0;
    else if (add_suf() == 0)
        return 1;
    else return 1;
}

int add_suf()
{
    printf("add_suf\n");
    if ( (recognize(plus) == 0) && (recognize(minus) == 0) )
        return 0;
    else if (add_suf() == 0)
        return 1;
    else return 1;
}

int multiplicative_expr()
{
    printf("Mult_expr\n");
    if (unary_expr() == 0)
        return 0;
    else if (mult_suf() == 0)
        return 1;
    else return 1;
}

int mult_suf()
{
    printf("Mult_suf\n");
    if ( (recognize(ast) == 0) && (recognize(slash) == 0) && (recognize(percent) == 0) )
        return 0;
    else if (mult_suf() == 0)
        return 1;
    else return 1;
}

int unary_expr()
{
    printf("unary_expr\n");
    if ( (recognize(minus) == 0) && (recognize(bang) == 0) && (recognize(bitand) == 0) &&
            (recognize(bitnot) == 0) )
    {
        if (primary_expr() == 0)
            return 0;
        else return 1;
    }
    else if (primary_expr() == 0)
        return syntax_error("primary expr after unary_expr\n");
        // exit(0);
    else return 1;
}

// SOMETHING HERE CAME BACK AS ZERO
int primary_expr()
{
    printf("Primary_expr\n");
    if (lhs() == 0)
    {
        if (recognize(READ_M) == 0)
        {
            if (recognize(WRITE_M) == 0)
            {
                if (recognize(deci) == 0)
                {
                    if (recognize(flt) == 0)
                    {
                        if(recognize(hexa) == 0)
                        {
                            if (recognize(oct) == 0)
                            {
                                if (recognize(STRING_M) == 0)
                                {
                                    if (recognize(lparenth) == 0)
                                    {
                                        if (recognize(ALLOC_M) == 0)
                                        {
                                            if (recognize(DEALLOC_M) == 0)
                                                return 0;
                                            else if (recognize(lparenth) == 0)
                                                return syntax_error("lparen after DEALLOC\n");
                                            else if (expr() == 0)
                                                return syntax_error("Expr after lparen dealloc\n");
                                            else if (recognize(rparenth) == 0)
                                                return syntax_error("rparen after expr for dealloc\n");
                                            else return 1;
                                        }
                                        else if (recognize(lparenth) == 0)
                                            return syntax_error("lparen after ALLOC\n");
                                        else if (expr() == 0)
                                            return syntax_error("Expr after lparen alloc\n");
                                        else if (recognize(rparenth) == 0)
                                            return syntax_error("rparen after expr for alloc\n");
                                        else return 1;
                                    }
                                    else if (expr() == 0)
                                        return syntax_error("Expr after lparen primary_expr\n");
                                    else if (recognize(rparenth) == 0)
                                        return syntax_error("Right paren after expr\n");
                                    else return 1;
                                }
                                else return 1;
                            }
                            else return 1;
                        }
                        else return 1;
                    }
                    else return 1;
                }
                else return 1;
            }
            else if (recognize(lparenth) == 0)
                return syntax_error("Left paren after WRITE\n");
            else if (expr_list() == 0)
                return syntax_error("EXPR LIST after left paren\n");
            else if (recognize(rparenth) == 0)
                return syntax_error("Right paren after LHS list\n");
            else return 1;
        }
        else if (recognize(lparenth) == 0)
            return syntax_error("Left paren after READ\n");
        else if (lhs_list() == 0)
            return syntax_error("LHS LIST after left paren\n");
        else if (recognize(rparenth) == 0)
            return syntax_error("Right paren after LHS list\n");
        else return 1;
    }
    else return 1;

}

int lhs()
{
    if (recognize(ID_M) == 0)
        return 0;
    else if (lhs_suf() == 0)
        return 1;
    else return 1;
}

int lhs_suf()
{
    if (recognize(lsbrace) == 0)
    {
        if (recognize(period) == 0)
        {
            if (recognize(lparenth) == 0)
                return 0;
            else if (opt_expr_list() == 0)
                return 1; // opt_expr_list() to always return 1
            else if (recognize(rparenth) == 0)
                return syntax_error("Right paren after (opt_expr_list)\n");
            else return 1;
        }
        else if (recognize(ID_M) ==0)
            return syntax_error("IDENTIFIER after dot\n");
        else if (lhs_suf() == 0)
            return 1;
        else return 1;
    }
    else if (expr() == 0)
        return syntax_error("Expression after lsbrace\n");
    else if (recognize(rsbrace) == 0)
        return syntax_error("rsbrace after expression\n");
    else if (lhs_suf() == 0)
        return 1;
    else return 1;
}

int lhs_list()
{
    if (lhs() == 0)
        return 0;
    else if (lhs_list_suf() == 0)
        return 1;
    else return 1;
}

int lhs_list_suf()
{
    if (recognize(comma) == 0)
        return 0;
    else if (lhs() == 0)
        return syntax_error("LHS after comma\n");
    else if (lhs_list_suf() == 0)
        return 1;
    else return 1;
}

int expr_list()
{
    if (expr() == 0)
        return 0;
    else if (expr_list_suf() == 0)
        return 1;
    else return 1;
}

int expr_list_suf()
{
    if (recognize(comma) == 0)
        return 0;
    else if (expr() == 0)
        return syntax_error("Expression after comma\n");
    else if (expr_list_suf() == 0)
        return 1;
    else return 1;
}

int opt_expr_list()
{
    if (expr_list() == 0)
        return 1;
    else return opt_expr_list();
}

int syntax_error(char* str)
{
    fprintf(stderr, str);
    return 1;
}