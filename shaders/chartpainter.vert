#version 450 core
layout (location = 0) in vec2 vertex;

uniform mat4 m_pv;
uniform float depth;
uniform vec2 tr;

void main(void) {
  gl_Position = m_pv * vec4(vertex + tr, depth, 1.);
}
