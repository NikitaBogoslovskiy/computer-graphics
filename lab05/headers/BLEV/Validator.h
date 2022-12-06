#ifndef __BLEV__VALIDATOR_H__
#define __BLEV__VALIDATOR_H__

namespace BLEV {
	class Validator {
	public:
		static void ValidateMeshGraphArgs(char* nstr, float& x0, float& x1, float& z0, float& z1, int& x_parts, int& z_parts);
		static void ValidateSphereArgs(char* nstr, double& x0, double& y0, double& z0, double& radius);
	};
}

#endif // !__BLEV__VALIDATOR_H__