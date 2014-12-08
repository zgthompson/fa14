TOKENS_TERMINAL = set([ "ERROR", "IDENT", "INTTYPE", "DBLTYPE", "NUMLIT", "PLUS", "MINUS", "MULT", "DIV", "MOD", "NOT", "AND", "OR", "XOR", 
"ASSIGN", "LT", "GT", "SHIFTL", "SHIFTR", "PLUSPLUS", "PLUSEQ", "MINUSMINUS", "MINUSEQ", "MULTEQ", "DIVEQ", "MODEQ", "NOTEQ", 
"LOGAND", "ANDEQ", "LOGOR", "OREQ", "XOREQ", "EQUALTO", "SHIFTLEQ", "LTE", "SHIFTREQ", "GTE", "TILDA", "RPAREN", "LPAREN", 
"SEMI", "QUEST", "COLON", "COMMA", "EXP", "EXPEQ", "EOFT" ])

TOKENS_NONTERMINAL = set([ "<program>", "<more_stmts>", "<stmt>", "<decl>", "<ntype>", "<decl_tail>", "<more_decls>", "<term>", "<pre>", 
"<uoppre>", "<var>", "<post>", "<stmt_tail>", "<binop>" ])

START = "<program>"

RULES = [
    ["<program>", "<stmt>", "SEMI", "<more_stmts>", "EOFT"], ["<program>", "<decl>", "SEMI", "<more_stmts>", "EOFT"], ["<more_stmts>", "<stmt>", "SEMI", "<more_stmts>"],
    ["<more_stmts>", "<decl>", "SEMI", "<more_stmts>"], ["<more_stmts>", None], ["<stmt>", "<term>", "<stmt_tail>"], ["<decl>", "<ntype>", "IDENT", "<decl_tail>", "<more_decls>"],
    ["<ntype>", "INTTYPE"], ["<ntype>", "DBLTYPE"], ["<decl_tail>", "ASSIGN", "<stmt>"], ["<decl_tail>", None], ["<more_decls>", "COMMA", "IDENT", "<decl_tail>", "<more_decls>"], 
    ["<more_decls>", None], ["<term>", "<pre>", "<var>", "<post>"], ["<term>", "LPAREN", "<stmt>", "RPAREN"], ["<pre>", "<uoppre>", "<pre>"], ["<pre>", None], ["<uoppre>", "PLUS"],
    ["<uoppre>", "MINUS"], ["<uoppre>", "NOT"], ["<uoppre>", "TILDA"], ["<uoppre>", "PLUSPLUS"], ["<uoppre>", "MINUSMINUS"], ["<var>", "IDENT"], ["<var>", "NUMLIT"], ["<post>", "PLUSPLUS"],
    ["<post>", "MINUSMINUS"], ["<post>", None], ["<stmt_tail>", "<binop>", "<stmt>"], ["<stmt_tail>", "QUEST", "<stmt>", "COLON", "<stmt>"], ["<stmt_tail>", None], ["<binop>", "PLUS"],
    ["<binop>", "MINUS"], ["<binop>", "MULT"], ["<binop>", "DIV"], ["<binop>", "MOD"], ["<binop>", "AND"], ["<binop>", "OR"], ["<binop>", "XOR"], ["<binop>", "ASSIGN"],
    ["<binop>", "LT"], ["<binop>", "GT"], ["<binop>", "SHIFTL"], ["<binop>", "SHIFTR"], ["<binop>", "PLUSEQ"], ["<binop>", "MINUSEQ"], ["<binop>", "MULTEQ"], ["<binop>", "DIVEQ"],
    ["<binop>", "MODEQ"], ["<binop>", "NOTEQ"], ["<binop>", "LOGAND"], ["<binop>", "ANDEQ"], ["<binop>", "LOGOR"], ["<binop>", "OREQ"], ["<binop>", "XOREQ"], ["<binop>", "EQUALTO"],
    ["<binop>", "SHIFTLEQ"], ["<binop>", "LTE"], ["<binop>", "SHIFTREQ"], ["<binop>", "GTE"], ["<binop>", "COMMA"], ["<binop>", "EXP"], ["<binop>", "EXPEQ"]
]

def firstsAndFollows(terminalTokens, nonterminalTokens, rules):

    firsts = { rule[0]: set() for rule in rules }
    follows = { rule[0]: set() for rule in rules }

    while True:

        didChange = False

        for rule in rules:

            # rule 1
            if rule[1] in terminalTokens and rule[1] not in firsts[rule[0]]:
                firsts[rule[0]].add(rule[1])
                didChange = True

            #rule 2
            if rule[1] in nonterminalTokens and not firsts[rule[1]].issubset(firsts[rule[0]]):
                firsts[rule[0]] = firsts[rule[0]].union(firsts[rule[1]])
                didChange = True

            #rule 3
            if rule[1] == None and not follows[rule[0]].issubset(firsts[rule[0]]):
                firsts[rule[0]] = firsts[rule[0]].union(follows[rule[0]])
                didChange = True

            for tokenIndex in range(1, len(rule) -1):
                curToken = rule[tokenIndex]
                nextToken = rule[tokenIndex+1]

                #rule 4
                if curToken in nonterminalTokens and nextToken in terminalTokens and nextToken not in follows[curToken]:
                    follows[curToken].add(nextToken)
                    didChange = True

                #rule 5
                if curToken in nonterminalTokens and nextToken in nonterminalTokens and not firsts[nextToken].issubset(follows[curToken]):
                    follows[curToken] = follows[curToken].union(firsts[nextToken])
                    didChange = True

            #rule 6
            if rule[-1] in nonterminalTokens and not follows[rule[0]].issubset(follows[rule[-1]]):
                follows[rule[-1]] = follows[rule[-1]].union(follows[rule[0]])

        if not didChange:
            break

    return firsts, follows

def formatAndPrint(d, title):
    print title
    keys = sorted(d.keys())
    for key in keys:
        if d[key]:
            print "%s: %s" % (key, ", ".join(sorted(list(d[key]))))


def main():
    firsts, follows = firstsAndFollows(TOKENS_TERMINAL, TOKENS_NONTERMINAL, RULES)

    formatAndPrint(firsts, "FIRSTS")
    formatAndPrint(follows, "FOLLOWS")

main()





