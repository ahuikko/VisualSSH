#ifndef TRANSFORM_CLASS_H
#define TRANSFORM_CLASS_H


#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "../math/vector3.h"
#include "../math/quaternion.h"



class Transform
{
public:
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;

    Transform() : position(0.0f, 0.0f, 0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) {} // default Transform = pos(0,0,0) rot(0,0,0) scale(1f,1f,1f)

    Transform(Vector3 pos, Quaternion rot, Vector3 scl)
        : position(pos), rotation(rot), scale(scl){}
        
       


};

#endif

