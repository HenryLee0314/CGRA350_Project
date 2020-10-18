#ifndef GRASS_PARAMETERS_H
#define GRASS_PARAMETERS_H

namespace CGRA350 {

class GrassParameters
{
public:
	static GrassParameters* getInstance();

	void renderGUI();

	inline float getWsCoefficient()
	{
		return Ws_coefficient;
	}

    inline float getRsCoefficient()
    {
    	return Rs_coefficient;
    }

	inline float getWbCoefficient()
	{
		return Wb_coefficient;
	}

	inline float getRbCoefficient()
	{
		return Rb_coefficient;
	}

	inline float getAngleCoefficient()
	{
		return Angle_coefficient;
	}

private:
	GrassParameters();
	virtual ~GrassParameters();
	GrassParameters(const GrassParameters&);
	GrassParameters& operator = (const GrassParameters&);

private:
	static GrassParameters* _instance;

    float Ws_coefficient;
    float Rs_coefficient;
	float Wb_coefficient;
	float Rb_coefficient;
	float Angle_coefficient;
};

} // namespace CGRA350

#endif // GRASS_PARAMETERS_H