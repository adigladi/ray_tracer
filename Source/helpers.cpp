#include "raytracer.h"

//
// Checks for intersection against all triangles, if found returns true, else false.
// If intersection found, then return info about the closest intersection.
//
bool ClosestIntersection(vec3 start, vec3 dir, const vector<Triangle>& triangles, Intersection& closestIntersection ) {
    float m = std::numeric_limits<float>::max();

    bool found = false;
    for( size_t i = 0; i < triangles.size(); ++i ) {
        vec3 v0 = triangles[i].v0;
        vec3 v1 = triangles[i].v1;
        vec3 v2 = triangles[i].v2;
        vec3 edge1 = v1 - v0;
        vec3 edge2 = v2 - v0;
        vec3 b = start - v0;
        mat3 A( -dir, edge1, edge2 );
        vec3 x = glm::inverse( A ) * b; // x = (t,u,v);


        // Check iniqualities (7), (8), (9) and (11)
        if(triangleIntersection(x)) {
            float u = x.y; float v = x.z;
            vec3 point = v0 + (edge1 * u) + (edge2 * v);
            float r = glm::distance(start, point);
            if(r < m) {
                    m = r;
                    closestIntersection.triangleIndex = i;
                    closestIntersection.position = point;
                    closestIntersection.distance = r;
                    found = true;
                }
        }
    }
    return found;
}



void GetRayDirection(int x, int y, vec3 &rayDir) {
    float u = x - (SCREEN_WIDTH / 2);
    float v = y - (SCREEN_HEIGHT / 2);
    rayDir.x = u;
    rayDir.y = v;
    rayDir.z = focalLength;
}


void printVector(const char* name, vec3 v) {
    cout << name << ": " << v.x << "," << v.y << "," << v.z << endl;
}


bool triangleIntersection(vec3 point) {
    float t = point.x;
    float u = point.y;
    float v = point.z;
    return (t >= 0 && u >= 0 && v >= 0 && (u + v) <= 1);
}



float rand_f(float min, float max) {
    return min + float(rand()) / float(RAND_MAX/(max - min));
}


// HOW TO RUN
//    for( float step = 0; step < result_size; step++ ) {
//            result[step] = Interpolate_f(start, end, step, result_size);
//    }
float Interpolate_f(float start, float end, float step, float max) {
    return (end - start) * (step / (max - 1)) + start;
}


// HOW TO RUN
// vector<vec3> result( 4 );
//     vec3 a(1,4,9.2);
//     vec3 b(4,1,9.8);
//     Interpolate_v( a, b, result );
//     for( int i=0; i<result.size(); ++i )
//     {
//         cout << "( "
//         << result[i].x << ", "
//         << result[i].y << ", "
//         << result[i].z << " ) ";
//     }
void Interpolate_v(vec3 a, vec3 b, vector<vec3> &result) {
    float max_size = result.size();
    for(float i = 0; i < max_size; i++) {
        result[i].x = Interpolate_f(a.x, b.x, i, max_size);
        result[i].y = Interpolate_f(a.y, b.y, i, max_size);
        result[i].z = Interpolate_f(a.z, b.z, i, max_size);
    }
}

// HOW TO RUN:
// vector<float> result( 12 ); // Create a vector width 10 floats
// Interpolate( 5, 14, result ); // Fill it with interpolated values
// for( double i = 0; i < result.size(); i++ )
//     cout << result[i] << " "; // Print the result to the terminal
void Interpolate( float start, float end, vector<float>& result ) {
    float result_size = (float) result.size();

    if(result_size < 2.0) {
        throw std::invalid_argument( "Interpolate size was too small");
    }

    for( float step = 0; step < result_size; step++ ) {
            result[step] = Interpolate_f(start, end, step, result_size);
    }
}

vec2 convertTo2D(vec3 coords) {
        float f = SCREEN_HEIGHT / 2;
        float u = f * coords.x / coords.z + SCREEN_WIDTH / 2;
        float v = f * coords.y / coords.z + SCREEN_HEIGHT / 2;
        return vec2(u, v);
}