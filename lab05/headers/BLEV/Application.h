#pragma once
#ifndef __BLEV_APPLICATION_H__
#define __BLEV_APPLICATION_H__

#include "Data.h"
#include "BLEV/Geometry.h"
#include "BLEV/Interface.h"

namespace BLEV {
	class Application {
		Data data;
	public:
		Interface body{ data };
	};
}



#endif // !__BLEV_APPLICATION_H__
