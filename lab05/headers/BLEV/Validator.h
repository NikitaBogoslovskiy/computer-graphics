#ifndef __BLEV__VALIDATOR_H__
#define __BLEV__VALIDATOR_H__

namespace BLEV {
	class Validator {
	public:
		static void ValidateMeshGraphArgs(char* nstr, float& x0, float& x1, float& z0, float& z1, int& x_parts, int& z_parts);
		static void ValidateSphereArgs(char* nstr, double& x0, double& y0, double& z0, double& radius);
		static void ValidateLightArgs(char* nstr, double& inPitch, double& inYaw, double& inR, double& inIntensity);
		static void ValidatePlaneArgs(char* nstr, double& x0, double& y0, double& z0, double& u, double& v);

		// actually no reason having different validating functions lmao
		/*static void ValidateEllipsoidArgs(char* nstr, double& x0, double& y0, double& z0, double& rX, double& rY, double& rZ);
		static void ValidateBoxArgs(char* nstr, double& x0, double& y0, double& z0, double& rX, double& rY, double& rZ);
		static void ValidateCylinderArgs(char* nstr, double& x0, double& y0, double& z0, double& rX, double& rY, double& rZ);
		static void ValidateCylinderArgs(char* nstr, double& x0, double& y0, double& z0, double& rX, double& rY, double& rZ);*/

		static void ValidateOS(char* nstr, double& x0, double& y0, double& z0, double& rX, double& rY, double& rZ);
	};
}

#endif // !__BLEV__VALIDATOR_H__