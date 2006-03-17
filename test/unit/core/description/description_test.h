#ifndef DESCRIPTION_TEST_H
#define DESCRIPTION_TEST_H

#undef NULL
#include <CuTest.h>
#include "test_op.h"
#include "test_svc.h"
#include "test_flow.h"
#include "test_handler_desc.h"

CuSuite* axis2_descriptionGetSuite();

#endif /* DESCRIPTION_TEST_H */
