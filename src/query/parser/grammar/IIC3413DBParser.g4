parser grammar IIC3413DBParser;

options {
    tokenVocab = IIC3413DBLexer;
}

root: query EOF;

query: createQuery | insertQuery | selectQuery;

createQuery: CREATE TABLE identifier '(' schema (',' schema)* ')';

insertQuery: INSERT INTO identifier VALUES '(' constant (',' constant)* ')';

selectQuery: selectStatement fromStatement whereStatement? limitStatement?;

schema: identifier datatype;

datatype: STR | INT;

selectStatement: SELECT DISTINCT? ('*' | columnList);

columnList: column (',' column)*;

fromStatement: FROM tableList;

tableList: table (',' table)*;

table: identifier (AS identifier)?;

whereStatement: WHERE andExpr;

andExpr: simpleExpr (AND simpleExpr)*;

simpleExpr: comparisonExpr
|           likeExpr
;

comparisonExpr: columnOrConstant (op=('=='|'!='|'<'|'>'|'<='|'>=') columnOrConstant)?;

likeExpr: column LIKE STRING;

columnOrConstant: column | constant;

column: identifier ('.' identifier)?;

constant: INTEGER | STRING;

identifier: IDENTIFIER | keyword;

limitStatement: LIMIT INTEGER;

keyword: AND
|        AS
|        CREATE
|        DISTINCT
|        FROM
|        INSERT
|        INTO
|        INT
|        LIKE
|        LIMIT
|        OR
|        SELECT
|        STR
|        TABLE
|        VALUES
|        WHERE
;
