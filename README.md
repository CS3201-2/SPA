# Team Sigma's CS3201/2 project

##SPA Introduction##

A Static Program Analyzer(SPA) is an interactive tool that automatically answers queries about programs.

##**SPA Requirements**##
####SIMPLE Language####
![SOURCE](http://i61.tinypic.com/2hoeq36.jpg)

####Program Design Abstraction####
Calls, Calls\*, Modifies, Modifies\*, Uses, Uses\*,

Follows, Follows\*, Parent, Parent\*, Next, Next\*, Affects, Affects\*

####Program Query Language (PQL)####
e.g. Select p such that Calls (p, q) with q.procName = “Third”

####Abstract Syntax Tree (AST)####
![AST](http://i59.tinypic.com/244w7j5.jpg)

##**SPA Architecture**##
![Architecture](http://i58.tinypic.com/2gseiro.jpg)

-**SPA Front-End**

   -*Parser*: extracts design abstractions including: 

              Uses, Modifies, ProcTable, VarTable, AST, Parent, Follows, Calls. 

   -*Design Extractor*: extracts remaining design abstractions

-**Program Knowledge Base(PKB)**

   -*data structures*: AST, Calls, CFG, Follows, Parent, Modifies, Uses, ProcTable, VarTable.

-**Query Processor**

   -*Query Preprocessor*
   
         -*Query Validator*: Checks if passed query is syntactically and semantically correct.
         
         -*Query Tree Creator*: Creates a query tree for the query.
   
   -*Query Evaluator*: Evaluates the query using the query tree and fetches the relevant data from the PKB.
   
   -*Query Projector*: Formats the results of the Query Evaluator into a user-friendly form.
