#pragma once

#include "Pendulum.h"

class WikiPendulum : public Pendulum
{
public:
    WikiPendulum(const std::string& name = "WikiPendulum",
        GameObject* parent = nullptr)
        : Pendulum(name, parent)
    {
        enableText(true);
        //enableEnergyCorrection(true);
    }
private:

	double getAngleAcceleration1(const PendulumData& p1, const PendulumData& p2) override
	{
        double m1 = p1.mass;
        double m2 = p2.mass;
        double l1 = p1.length;
        double l2 = p2.length;
        double g = m_gravity;
      //  double a1 = p1.angleAcceleration;
      //  double a2 = p2.angleAcceleration;
        double v1 = p1.angleVelocity;
        double v2 = p2.angleVelocity;
        double phi1 = p1.angle;
        double phi2 = p2.angle;

        /*double lm = -m2 / (m1 + m2) * l2 / l1;
        double sum1 = a2 * cos(phi1 - phi2);
        double sum2 = v2 * v2 * sin(phi1 - phi2);
        double sum3 = -g / l1 * sin(phi1);

        double acc = lm * (sum1 + sum2) + sum3;
        return acc;*/
        double a1 = ((-(g * (m1 * sin(phi1) - m2 * (cos(phi1 - phi2) * sin(phi2) - sin(phi1))) + 
                       (l1 * cos(phi1 - phi2) * v1 * v1 + l2 * v2*v2) * m2 * sin(phi1 - phi2))) / 
                        (l1 * (m1 + m2 * (sin(phi1 - phi2) * sin(phi1 - phi2)))));
        return a1;
	}
	double getAngleAcceleration2(const PendulumData& p1, const PendulumData& p2) override
	{
        double m1 = p1.mass;
        double m2 = p2.mass;
        double l1 = p1.length;
        double l2 = p2.length;
        double g = m_gravity;
        //double a1 = p1.angleAcceleration;
        //double a2 = p2.angleAcceleration;
        double v1 = p1.angleVelocity;
        double v2 = p2.angleVelocity;
        double phi1 = p1.angle;
        double phi2 = p2.angle;

        /*double l = -l1 / l2;
        double sum1 = a1 * cos(phi1 - phi2);
        double sum2 = -v1 * v1 * sin(phi1 - phi2);
        double sum3 = -g / l2 * sin(phi2);

        double acc = l * (sum1 + sum2) + sum3;
        return acc;*/
        double a2 = ((g * (m1 + m2) * (sin(phi1) * cos(phi1 - phi2) - sin(phi2)) +
            (l1 * (m1 + m2) * v1 * v1 + l2 * m2 * cos(phi1 - phi2) * v2 * v2) * sin(phi1 - phi2)) /
            (l2 * (m1 + m2 * (sin(phi1 - phi2) * sin(phi1 - phi2)))));
        return a2;
	}


};