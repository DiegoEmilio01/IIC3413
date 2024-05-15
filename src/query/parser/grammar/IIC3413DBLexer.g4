lexer grammar IIC3413DBLexer;

channels {
    WS_CHANNEL
}

AND:      A N D;
AS:       A S;
CREATE:   C R E A T E;
DISTINCT: D I S T I N C T;
FROM:     F R O M;
INSERT:   I N S E R T;
INTO:     I N T O;
INT:      I N T;
LIKE:     L I K E;
LIMIT:    L I M I T;
OR:       O R;
SELECT:   S E L E C T;
STR:      S T R;
TABLE:    T A B L E;
VALUES:   V A L U E S;
WHERE:    W H E R E;

EQ: '==';
NE: '!=';
GT: '>';
LT: '<';
GE: '>=';
LE: '<=';

COMMA: ',';
ASTERISK: '*';
MINUS: '-';
SLASH: '/';
PLUS: '+';
DOT: '.';
L_PAR: '(';
R_PAR: ')';

INTEGER: MINUS? DIGIT+;

STRING: '"' (~('"' | '\\' | '\n' | '\r') | ECHAR)* '"';

IDENTIFIER: [A-Za-z][A-Za-z0-9_]*;

WHITE_SPACE: [ \t\r\n]+ -> channel(WS_CHANNEL);

COMMENT_INPUT      : '/*' .*? '*/'  -> channel(HIDDEN);
LINE_COMMENT:
    (('--' [ \t]* | '#') ~[\r\n]* ('\r'? '\n' | EOF) | '--' ('\r'? '\n' | EOF)) -> channel(HIDDEN)
;

UNRECOGNIZED: .;

fragment ECHAR: '\\' ('t' | 'b' | 'n' | 'r' | 'f' | '\\' | '"' | '\'');

fragment DIGIT: [0-9];

fragment A: [aA];
fragment B: [bB];
fragment C: [cC];
fragment D: [dD];
fragment E: [eE];
fragment F: [fF];
fragment G: [gG];
fragment H: [hH];
fragment I: [iI];
fragment J: [jJ];
fragment K: [kK];
fragment L: [lL];
fragment M: [mM];
fragment N: [nN];
fragment O: [oO];
fragment P: [pP];
fragment Q: [qQ];
fragment R: [rR];
fragment S: [sS];
fragment T: [tT];
fragment U: [uU];
fragment V: [vV];
fragment W: [wW];
fragment X: [xX];
fragment Y: [yY];
fragment Z: [zZ];
