import re
import collections

class LexicalAnalyzer(object):

    Token = collections.namedtuple('Token', ['name', 'value'])

    token_map = { '+': 'PLUS', '-': 'MINUS', '*': 'MULT', '/': 'DIV', '%': 'MOD',
            '!': 'NOT', '&': 'AND', '|': 'OR', '^': 'XOR', '=': 'ASSIGN',
            '<': 'LT', '>': 'GT', '<<': 'SHIFTL', '>>': 'SHIFTR', '++': 'PLUSPLUS',
            '+=': 'PLUSEQ', '--': 'MINUSMINUS', '-=': 'MINUSEQ', '*=': 'MULTEQ',
            '/=': 'DIVEQ', '%=': 'MODEQ', '!=': 'NOTEQ', '&&': 'LOGAND',
            '&=': 'ANDEQ', '||': 'LOGOR', '|=': 'OREQ', '^=': 'XOREQ',
            '==': 'EQUALTO', '<<=': 'SHIFTLEQ', '<=': 'LTE', '>>=': 'SHIFTREQ',
            '>=': 'GTE', '~': 'TILDA', ')': 'RPAREN', '(': 'LPAREN', ';': 'SEMI',
            '?': 'QUEST', ':': 'COLON', ',': 'COMMA', '**': 'EXP', '**=': 'EXPEQ',
            'int ': 'INTTYPE', 'double ': 'DBLTYPE' }

    numeric_literal_regex = '[\d.]+'
    identifier_regex = '[_A-Za-z][_0-9A-Za-z]*'
    token_map_regex = '|'.join( [ re.escape(token) for token in sorted(token_map.keys(), key=len, reverse=True) ] )

    token_regex = '%s|%s|%s' % (token_map_regex, numeric_literal_regex, identifier_regex)

    def __init__(self, filename):
        self.filename = filename

    def tokens(self):
        with open(self.filename) as f:
            split_text = re.findall(LexicalAnalyzer.token_regex, f.read())
            return [ LexicalAnalyzer.Token( LexicalAnalyzer.token_map.get(lexeme, LexicalAnalyzer.other_lexeme_type(lexeme)), lexeme ) for lexeme in split_text ]

    @staticmethod
    def other_lexeme_type(lexeme):
        if re.match(LexicalAnalyzer.numeric_literal_regex, lexeme):
            return 'NUMLIT'
        elif re.match(LexicalAnalyzer.identifier_regex, lexeme):
            return 'IDENT'
        else: 
            return 'ERROR'

