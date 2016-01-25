### Design structure
- input
- lexer/tokenizer
- non-linear, keep track on all sets
- parser
- ast creator?
- halfway interpreter
- interpreter/compiler



### ToDo:
- Implement float to lex
- Implement string to lex
- Implement scopes in lex in a recursive manner



####TokenCategories
- symbol
- int
- float
- string
- arithmetic
- scope
- separator
- spacing
- terminator




#### TokenTypes

Symbol
- function
- variable

Arithmetic
- plus (+)
- minus (-)
- multiplication (\*)
- division (/)
- modulo (%)
- equals (==)
- increase (+=)
- lower (-=)
- mutiply(\*=)
- power (\*\*)

Raw 32-bit types
- integer
- float

