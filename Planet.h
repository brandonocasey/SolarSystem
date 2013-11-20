#pragma once
#include "BaseSphere.h"

class Planet : public BaseSphere
{
public:
    void InitRotation( bool rotation, int rotation_speed, float radius );
    void InitOrbit( bool orbit, int orbit_speed, int orbit_distance, BaseSphere* orbit_oibject);
    void Draw();
    void Update();
private:
    bool m_bOrbit;
    BaseSphere* m_cOrbitObject;
    int m_iOrbitSpeed;
    int m_iOrbitDistance;
};