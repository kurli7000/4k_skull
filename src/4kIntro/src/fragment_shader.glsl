#version 430

layout (location=0) out vec4 fragColor;
layout (location=0) uniform vec4 fpar[4];
uniform sampler2D tex1;

in vec2 p;

const float epsilon = 0.000001;
const float bigf = 1000000.0;
const float exposure = 0.8;
const float planeDistance = 0.1;
const float cameraDistance = 2.5;
const float planeScale = 0.07;
const vec3 camPos = vec3(1.0, 0.4, cameraDistance);
const vec3 target = vec3(0.0, 0.5, 0.75);
const int max_depth = 10;
const vec4 light0 = vec4(0.0, 5.0, 0.0, 2.0);
const int focalBlurSteps = 16;
const float focalBlurStep = 0.003;


bool Plane(vec3 rayOrigin, vec3 rayDir, vec4 plane, out vec4 p)
{ 
    if (abs(dot(plane.xyz, rayDir)) < epsilon) return false;
    
	float t = -(dot(plane.xyz, rayOrigin) - plane.w) / dot(plane.xyz, rayDir);
    p.xyz = rayOrigin + t * rayDir;
    p.w = t;
    
    if (t < 0.0) return false;
    
    return true;
}


bool Triangle(vec3 rayOrigin, vec3 rayDir, vec3 v0, vec3 v1, vec3 v2, out vec4 p)
{
    vec3 v0v1 = v1 - v0; 
    vec3 v0v2 = v2 - v0; 
    vec3 pvec = cross(rayDir, v0v2); 
    float det = dot(v0v1, pvec); 
	float u, v, t;
    
    if (abs(det) < epsilon) return false;
    
    float invDet = 1.0 / det; 
 
    vec3 tvec = rayOrigin - v0;
    u = dot(tvec, pvec) * invDet; 
    if (u < 0.0 || u > 1.0) return false; 
 
    vec3 qvec = cross(tvec, v0v1); 
    v = dot(rayDir, qvec) * invDet;
    if (v < 0.0 || u + v > 1.0) return false; 
 
    t = dot(v0v2, qvec) * invDet;
    
    if (t < 0.0) return false;
    
    p.xyz = rayOrigin + t * rayDir;
    p.w = t;
    
    return true; 
}


bool Sphere(vec3 rayOrigin, vec3 rayDir, vec4 sph, out vec4 p, out vec3 n)
{
    vec3 r2s = sph.xyz - rayOrigin;
    float r2 = sph.w * sph.w;
    float d1 = dot(rayDir, r2s);
    if (d1 < 0.0) return false;
    float d2 = dot(r2s, r2s) - d1 * d1;
    if (d2 > r2) return false;
    float d3 = sqrt(r2 - d2);
    float t = d1 - d3;
    //if (t < 0.0) t = d1 + d3;
    if (t < 0.0) return false;
    p.xyz = rayOrigin + t * rayDir;
    p.w = t;
    n = normalize(p.xyz - sph.xyz);
    return true;
}




// totally fake news
float SphereShadowFloor(vec3 pos, vec4 sph)
{
    vec3 sp2l = light0.xyz - sph.xyz;
    vec3 f2sp = sph.xyz - pos;
    float d = f2sp.y / sp2l.y;
    vec3 shadowCenter = sph.xyz + sp2l * d;
    vec3 c2pos = shadowCenter - pos;
    
    float minRad = sph.w - light0.w * d;
    float maxRad = sph.w + light0.w * d;
    float l = length(c2pos.xz);
    
    if (minRad >= maxRad) return 0.0;
    return smoothstep(maxRad, minRad, l);
}


vec4 ball(int i, float v)
{
	vec2 uv = vec2(float(i) *  fpar[0].y, v);
	return texture(tex1, uv);
}

bool scene(vec3 rayOrigin, vec3 rayDir, out vec4 p, out vec3 n, out vec4 color)
{
	bool hit = false;
    p = vec4(0.0, 0.0, 0.0, bigf);
    
    // floor plane
    {
    	vec4 p2;
        
        if (Plane(rayOrigin, rayDir, vec4(0.0, 1.0, 0.0, 0.0), p2))
        {
            hit = true;
            if (p2.w < p.w)
            {
                p = p2;
                n = vec3(0.0, 1.0, 0.0);

                float sh = 0.0;
                float sh2 = 0.0;
                float minDist = bigf;
                for (int i = 0; i < int(fpar[0].x); i++)
                {
					vec4 b = ball(i, 0.25);
                    sh += SphereShadowFloor(p2.xyz, b);
                    sh += SphereShadowFloor(p2.xyz, b * vec4(-1.0, 1.0, 1.0, 1.0));
                }
                
                for (int i = 0; i < int(fpar[0].z); i++)
                {
                    sh2 += SphereShadowFloor(p2.xyz, ball(i, 0.75));
                }

                float br = 1.0 - sqrt(sh * 0.03) - (sh2 * 0.5);
                br *= dot(n, normalize(light0.xyz - p2.xyz));
                color = vec4(br, br, br, 0.0);
            }
        }
    }
    
    // walls
    {
    	vec4 walls[5];
		walls[0] = vec4(1.0, 0.0, 0.0, -5.0);
		walls[1] = vec4(-1.0, 0.0, 0.0, -5.0);
		walls[2] = vec4(0.0, 0.0, 1.0, -5.0);
		walls[3] = vec4(0.0, 0.0, -1.0, -5.0);
		walls[4] = vec4(0.0, -1.0, 0.0, -10.0);
    	vec4 p2;
         
        for (int i = 0; i < 5; i++)
        {
            if (Plane(rayOrigin, rayDir, walls[i], p2))
            {
                hit = true;
                if (p2.w < p.w)
                {
                    p = p2;
                    n = walls[i].xyz;
                    float br = dot(n, normalize(light0.xyz - p2.xyz));
                    color = vec4(br, br, br, 0.0);
                }
            }                  
        }
    }
    
    // spheres
    for (int i = 0; i < int(fpar[0].x); i++)
    {
    	vec4 p2;
        vec3 n2;
        vec4 b = ball(i, 0.25);

        if (Sphere(rayOrigin, rayDir, b, p2, n2) || Sphere(rayOrigin, rayDir, b * vec4(-1.0, 1.0, 1.0, 1.0), p2, n2))
        {
            hit = true;
            if (p2.w < p.w)
            {
                p = p2;
                n = n2;
                float f = pow(dot(rayDir, reflect(rayDir, n)) * 0.5 + 0.5, 3.0);
                //float d = clamp(dot(n, normalize(light0.xyz - p2.xyz)), 0.0, 1.0);
                color = vec4(f, f, f, 1.0);
            }
        }
    }
    
    // stuff
    for (int i = 0; i < int(fpar[0].z); i++)
    {
    	vec4 p2;
        vec3 n2;
        
        if (Sphere(rayOrigin, rayDir, ball(i, 0.75), p2, n2))
        {
            hit = true;
            if (p2.w < p.w)
            {
                p = p2;
                n = n2;
                float f = pow(dot(rayDir, reflect(rayDir, n)) * 0.5 + 0.5, 3.0);
                //float d = clamp(dot(n, normalize(light0.xyz - p2.xyz)), 0.0, 1.0);
                f = f * 0.1 + 0.9;
                color = vec4(f, f, f, 1.0);
            }
        }
    }
    
    
    // light
    {
        vec4 p2;
        vec3 l0 = light0.xyz - vec3( light0.w, 0.0,  light0.w);
        vec3 l1 = light0.xyz - vec3(-light0.w, 0.0,  light0.w);
        vec3 l2 = light0.xyz - vec3(-light0.w, 0.0, -light0.w);
        vec3 l3 = light0.xyz - vec3( light0.w, 0.0, -light0.w);
        
        if (Triangle(rayOrigin, rayDir, l0, l1, l2, p2) || Triangle(rayOrigin, rayDir, l0, l2, l3, p2))
        {
            float br = 8.0;
            color = vec4(br, br, br, 0.0);
        }
    }
    
	
    return hit;
}


vec4 calc_pixel(vec3 rayOrigin, vec3 rayDir)
{
	vec4 p;
    vec3 n;
    vec3 color = vec3(1.0);
    vec3 ro = rayOrigin;
    vec3 rd = rayDir;
    
    for (int i = 0; i < max_depth; i++)
    {
        vec4 hitCol;
    	bool hit = scene(ro, rd, p, n, hitCol);
        
        if (hit)
        {
            color *= hitCol.rgb;
            rd = normalize(reflect(rd, n));
            ro = p.xyz + rd * 0.0001;
            if (hitCol.a < 0.5) break;
        }
        else
        {
            color = vec3(1.0);
            break;
        }
    }
    
	return vec4(color * exposure, 1.0);
}


void main() 
{
	vec4 color = vec4(0.0);
    vec2 plane = p.xy * vec2(1.0, 9.0 / 16.0);
 	plane *= planeScale;
    float r = float(focalBlurSteps) * 0.5;
    float a = 1.0 / (3.14159 * r * r);
    //float vignette = length(plane) * 6.0;
    //vignette = 1.0 - vignette * vignette;
    
    for (int y = 0; y < focalBlurSteps; y++)
    {
        float yy = float(y) - float(focalBlurSteps - 1) / 2.0;
        
        for (int x = 0; x < focalBlurSteps; x++)
    	{
            float xx = float(x) - float(focalBlurSteps - 1) / 2.0;
            
            if (xx * xx + yy * yy <= r * r)
            {
                vec3 cp = camPos;
                vec3 forward = normalize(target - cp);
                vec3 right = cross(forward, vec3(0.0, 1.0, 0.0));	
                vec3 up = cross(forward, right);
                cp = cp + xx * focalBlurStep * right + yy * focalBlurStep * up;
                forward = normalize(target - cp);

                vec3 rayOrigin = cp - right * plane.x - up * plane.y + forward * planeDistance;
                vec3 rayDir = normalize(rayOrigin - cp);
                color += calc_pixel(rayOrigin, rayDir) * vec4(1.0 - yy * 0.1, 1.0, 1.0 - xx * 0.1, 1.0);
            }
            
        }
    }
     
    fragColor = color * a;// * vignette;
}
