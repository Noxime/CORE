#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 fPosition;
layout(location = 1) in vec2 fTexcoord0;
layout(location = 2) in vec2 fTexcoord1;
layout(location = 3) in vec3 fNormal;
layout(location = 4) in vec3 fTangent;

layout(location = 0) out vec4 outColor;

void main() {
	outColor = vec4(0, 1, 0, 1);
}