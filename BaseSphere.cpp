#include "BaseSphere.h"

void BaseSphere::Init( bool rotate, bool orbit, int rotation_speed, int orbit_speed, int orbit_distance, int radius )
{
    m_bRotation = rotate;
    m_bOrbit = orbit;
    m_iRotationSpeed = rotation_speed;
    m_iOrbitSpeed = orbit_speed;
    m_iOrbitDistance = orbit_distance;
    m_iRadius = radius;
}

void BaseSphere::InitNoOrbit( bool rotate, int rotation_speed, int radius )
{

}

void BaseSphere::Update()
{
    glPushMatrix();
    // red
    glColor3f(1, 0, 0);
    // Make the sphere spin here
    if( m_bRotation )
    {
        //rotate sphere
        glRotatef(m_iRotationSpeed*360, 0, 1, 0);
    }

    if( m_bOrbit )
    {
        glTranslatef(m_iOrbitSpeed, m_iOrbitDistance, 0);
        // orbit around object;
    }
    glusphere(quad[0], m_iRadius, 64, 64);
    glPopMatrix();
}

void BaseSphere::Draw()
{

}

BaseSphere::BaseSphere()
{

}

BaseSphere::~BaseSphere()
{

}

void BaseSphere::GetLocation( int *x, int *y, int *z )
{

}
