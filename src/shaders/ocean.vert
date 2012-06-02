uniform mat4 rangec;

void intersectXZ(in vec3 o, in vec3 r, out vec3 p)
{
    vec3 n = vec3(0.0, 1.0, 0.0);
    float t = -dot(n, o) / dot(n, r);
    p = o + r*t;
}

void main(void)
{
    mat4 pm = gl_ModelViewProjectionMatrixInverse * rangec;
    vec4 p0 = pm * vec4(gl_Vertex.xy,-1.0, 1.0); 
    vec4 p1 = pm * vec4(gl_Vertex.xy, 1.0, 1.0);
    vec3 o = p0.xyz/p0.w;
    vec3 r = normalize(p1.xyz/p1.w - o);
    vec3 pos;
    intersectXZ(o, r, pos);
    gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0);
}
