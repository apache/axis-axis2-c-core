#ifndef OM_NODE_TEST_H
#define OM_NODE_TEST_H


#include <CuTest.h>
#include <axis2_env.h>
#include <axis2_om_node.h>

void Testaxis2_om_node_add_child(CuTest *tc);
void Testaxis2_om_node_set_parent(CuTest *tc);
void Testaxis2_om_node_insert_sibling_after(CuTest *tc);
void Testaxis2_om_node_insert_sibling_before(CuTest *tc);
void Testaxis2_om_node_detach(CuTest *tc);

#endif /* OM_NODE_TEST_H */
