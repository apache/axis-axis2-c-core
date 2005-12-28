#ifndef CALC_SKELETON_H
#define CALC_SKELETON_H

#include "calc.h"


	int AXISCALL invoke(void* pMsg);
	void AXISCALL onFault(void* pMsg);
	int AXISCALL init();
	int AXISCALL fini();
	AXIS_BINDING_STYLE AXISCALL getBindingStyle(){return RPC_ENCODED;};
private:/*Methods corresponding to the web service methods*/
	int EchoAttachment(void* pMsg);
    int EchoAttachments(void* pMsg);

};

#endif /* CALC_SKELETON_H */
