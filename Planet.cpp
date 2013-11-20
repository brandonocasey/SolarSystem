#include "Planet.h"

void Planet::InitRotation( bool rotation, int rotation_speed, float radius )
{
    m_bRotation = rotation;
    m_iRotationSpeed = rotation_speed;
    m_dRadius = radius;
}

void Planet::InitOrbit( bool orbit, int orbit_speed, int orbit_distance, BaseSphere* orbit_object )
{
    m_bOrbit = orbit;
    m_iOrbitSpeed = orbit_speed;
    m_iOrbitDistance = orbit_distance;
    m_cOrbitObject = orbit_object;

    orbit_object->GetLocation(&m_iPositionx, &m_iPositiony, &m_iPositionz);

    m_iPositionx += m_iOrbitDistance;
}

void Planet::Draw()
{
    //glTranslatef(m_iOrbitDistance, 0, 0);
}

void Planet::Update()
{

}
