import collections

class SyntacticAnalyzer(object):

    RuleMatch = collections.namedtuple('RuleMatch', ['name', 'matched'])
    rule_map = None # lazy loaded by initializer

    def __init__(self):
        if not SyntacticAnalyzer.rule_map:
            SyntacticAnalyzer.rule_map = SyntacticAnalyzer.build_rule_map()

    @staticmethod
    def build_rule_map():
        rule_map = collections.defaultdict(list)
        with open("grammar.txt") as f:
            for line in f:
                line_split = line.split()
                rule_map[ line_split[0] ].append( " ".join(line_split[2:]))
        return rule_map

    def match(self, rule_name, tokens):
        if tokens and rule_name == "{}":
            return SyntacticAnalyzer.RuleMatch("{}", tokens)
        if tokens and rule_name == tokens[0].name:
            return SyntacticAnalyzer.RuleMatch(tokens[0], tokens[1:])
        for expansion in SyntacticAnalyzer.rule_map.get(rule_name, ()):
            remaining_tokens = tokens
            matched_subrules = []
            for subrule in expansion.split():
                matched, remaining_tokens = self.match(subrule, remaining_tokens)
                if not matched:
                    break
                matched_subrules.append(matched)
            else:
                return SyntacticAnalyzer.RuleMatch(rule_name, matched_subrules), remaining_tokens
        return None, None

import lexical_analyzer

def main():
    la = lexical_analyzer.LexicalAnalyzer("test.txt")
    sa = SyntacticAnalyzer()
    match = sa.match("<program>", la.tokens())
    print match

main()
