
#include <iostream>
#include <string>
#include <unordered_map>

#include "../ParseTree.h"
#include "../optimizer/Print.h"
#include "../optionals/Relation.h"
#include "../tpch/Generator.h"
#include "../optimizer/QueryPlan.h"

// SELECT SUM DISTINCT (a.b + b), d.g FROM a AS b WHERE ('foo' > this.that OR 2
// = 3) AND (12 > 5) GROUP BY a.f, c.d, g.f

extern struct FuncOperator *finalFunction;
extern struct TableList *tables;
extern struct AndList *whereList;
extern struct NameList *groupingAtts;
extern struct NameList *attsToSelect;
extern int distinctAtts;
extern int distinctFunc;

extern "C" {
int yyparse(void); // defined in y.tab.c
}

int main() {
  yyparse();
  // PrintParsedSQL();
  QueryPlan plan;
  plan.GenerateSelectFiles();

}
