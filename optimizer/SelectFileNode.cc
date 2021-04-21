#include "SelectFileNode.h"
#include "Node.h"

SelectFileNode::SelectFileNode(Schema &schema, int outPipeID,
                               std::string aliasRelName)
    : numToSkip(0), outputSchema(schema), Node(outPipeID, aliasRelName) {

  std::string tempAttrName = this->outputSchema.GetAtts()[0].name;

  char *selectAllCNF =
      strdup(("(" + tempAttrName + " = " + tempAttrName + ")").c_str());

  this->selection.GrowSimpleFromSchema(this->outputSchema);
  free(selectAllCNF);
}