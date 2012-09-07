uniform mat4 rangec;
uniform sampler1D wave_texture;
uniform float num_waves;
uniform float time;

void intersectXZ(in vec3 o, in vec3 r, out vec3 p)
{
    vec3 n = vec3(0.0, 1.0, 0.0);
    float t = -dot(n, o) / dot(n, r);
    p = o + r*t;
}

void main(void)
{
    // project the screen space vertex onto the horizontal plane
    mat4 pm = gl_ModelViewProjectionMatrixInverse * rangec;
    vec4 p0 = pm * vec4(gl_Vertex.xy,-1.0, 1.0); 
    vec4 p1 = pm * vec4(gl_Vertex.xy, 1.0, 1.0);
    vec3 o = p0.xyz/p0.w;
    vec3 r = normalize(p1.xyz/p1.w - o);
    vec3 pos;
    intersectXZ(o, r, pos);

    // perturb the position
    for (float i = 0.0; i < num_waves; i += 1.0) {
        vec4 wave = texture1D(wave_texture, (i + 0.5)/num_waves);
        vec2 k = wave.zw;
        float term = wave.y*time - dot(k, pos.xz);
        vec2 dh = normalize(k)*wave.x*sin(term);
        float dy = wave.x*cos(term);
        pos += vec3(dh.x, dy, dh.y);
    }

    gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0);
}
