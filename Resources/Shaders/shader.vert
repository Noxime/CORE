#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexcoord0;
layout(location = 2) in vec2 vTexcoord1;
layout(location = 3) in vec3 vNormal;
layout(location = 4) in vec3 vTangent;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;


layout(location = 0) out vec3 fPosition;
layout(location = 1) out vec2 fTexcoord0;
layout(location = 2) out vec2 fTexcoord1;
layout(location = 3) out vec3 fNormal;
layout(location = 4) out vec3 fTangent;



out gl_PerVertex {
	vec4 gl_Position;
};



void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(vPosition, 1.0);
	//gl_Position = vec4(vPosition, 1.0);
	fPosition  = vPosition;
	fTexcoord0 = vTexcoord0;
	fTexcoord1 = vTexcoord1;
	fNormal    = vNormal;
	fTangent   = vTangent;
	
}