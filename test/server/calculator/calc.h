#ifndef CALC_H
#define CALC_H


#include <axis2_svc_skeleton.h>
#include <axis2_om_node.h>

axis2_om_node_t *add(axis2_om_node_t *node);
axis2_om_node_t *multiply(axis2_om_node_t *node);

#endif /* CALC_H*/
