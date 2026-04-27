#version 410
layout (location = 0) in vec3 position;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf;

out vec4 varying_color;

mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);
mat4 buildScale(float x, float y, float z);

void main(void)
{
    float i = gl_InstanceID + tf;
    float a = sin(203.0 * i / 8000.0) * 403.0;
    float b = sin(301.0 * i / 4001.0) * 401.0;
    float c = sin(400.0 * i / 6003.0) * 405.0;

    mat4 local_rot_x = buildRotateX(1000 * i);
    mat4 local_rot_y = buildRotateY(1000 * i);
    mat4 local_rot_z = buildRotateZ(1000 * i);
    mat4 local_trans = buildTranslate(a, b, c);

    mat4 new_m_martix = local_trans * local_rot_x * local_rot_y * local_rot_z;
    mat4 mv_matrix = v_matrix * new_m_martix;

    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    varying_color = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}

mat4 buildRotateX(float rad) {
    mat4 matrix = mat4(1.0, 0.0, 0.0, 0.0,
		        0.0, cos(rad), -sin(rad), 0.0,
		        0.0, sin(rad), cos(rad), 0.0,
		        0.0, 0.0, 0.0, 1.0);
    return matrix;
}

mat4 buildRotateY(float rad) {
    mat4 matrix = mat4(cos(rad), 0.0, sin(rad), 0.0,
		        0.0, 1.0, 0.0, 0.0,
		        -sin(rad), 0.0, cos(rad), 0.0,
		        0.0, 0.0, 0.0, 1.0);
    return matrix;
}

mat4 buildRotateZ(float rad) {
    mat4 matrix = mat4(cos(rad), sin(rad), 0.0, 0.0,
		        -sin(rad), cos(rad), 0.0, 0.0,
		        0.0, 0.0, 1.0, 0.0,
		        0.0, 0.0, 0.0, 1.0);
    return matrix;
}

mat4 buildTranslate(float x, float y, float z) {
    mat4 matrix = mat4(1.0, 0.0, 0.0, 0.0,
		        0.0, 1.0, 0.0, 0.0,
		        0.0, 0.0, 1.0, 0.0,
		        x, y, z, 1.0 );
    return matrix;
}

mat4 buildScale(float x, float y, float z){
mat4 matrix = mat4(x, 0.0, 0.0, 0.0, 
    0.0, y, 0.0, 0.0,
    0.0, 0.0, z, 0.0,
    0.0, 0.0, 0.0, 1.0);
    return matrix;
}