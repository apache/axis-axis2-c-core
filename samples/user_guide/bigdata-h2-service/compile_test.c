#include <stdio.h>
#include <stdlib.h>

// Test if we can compile with same includes as bigdata_h2_service.h but without linking axiom
#include <axutil_env.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axutil_string.h>
#include <axis2_msg_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_svc_skeleton.h>

// Note: We include the axiom header (this is allowed) but won't link to axiom library
#include <axiom_node.h>

// JSON processing
#include <json-c/json.h>

int main() {
    printf("✅ Compilation test successful!\n");
    printf("Headers can be included but library linking is separate\n");

    // Test json-c functionality (proving we have the JSON library)
    json_object *test_obj = json_object_new_string("HTTP/2 JSON Test");
    if (test_obj) {
        printf("✅ JSON-C library works: %s\n", json_object_get_string(test_obj));
        json_object_put(test_obj);
    }

    return 0;
}