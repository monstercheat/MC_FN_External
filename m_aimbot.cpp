#include "m_dll.hpp"
#include "m_kernel.hpp"

std::mutex fuck;


class mouse
{
public:
		
	inline void move(fvector2d Head2D) {
		float x = Head2D.x; float y = Head2D.y;
		float AimSpeed = g_smooth;

		fvector2d ScreenCenter = { (double)screen_width / 2 , (double)screen_height / 2 };
		fvector2d Target;

		if (x != 0)
		{
			if (x > ScreenCenter.x)
			{
				Target.x = -(ScreenCenter.x - x);
				Target.x /= AimSpeed;
				if (Target.x + ScreenCenter.x > ScreenCenter.x * 2) Target.x = 0;
			}

			if (x < ScreenCenter.x)
			{
				Target.x = x - ScreenCenter.x;
				Target.x /= AimSpeed;
				if (Target.x + ScreenCenter.x < 0) Target.x = 0;
			}
		}
		if (y != 0)
		{
			if (y > ScreenCenter.y)
			{
				Target.y = -(ScreenCenter.y - y);
				Target.y /= AimSpeed;
				if (Target.y + ScreenCenter.y > ScreenCenter.y * 2) Target.y = 0;
			}

			if (y < ScreenCenter.y)
			{
				Target.y = y - ScreenCenter.y;
				Target.y /= AimSpeed;
				if (Target.y + ScreenCenter.y < 0) Target.y = 0;
			}
		}
		if ((int)Target.x != 0 or (int)Target.y != 0);
		Move((int)Target.x, (int)Target.y);
		
		//else if (g_kernel)
		//{
		//	DD_movR((int)Target.x, (int)Target.y);
		//}
	}
	
};  static mouse* input = new mouse();

fvector PredictLocation(fvector target, fvector targetVelocity, float projectileSpeed, float projectileGravityScale, float distance)
{
	float horizontalTime = distance / projectileSpeed;
	float verticalTime = distance / projectileSpeed;

	target.x += targetVelocity.x * horizontalTime;
	target.y += targetVelocity.y * horizontalTime;
	target.z += targetVelocity.z * verticalTime +
		abs(-980 * projectileGravityScale) * 0.5f * (verticalTime * verticalTime);

	return target;
}