#version 410

attribute highp vec4 vertex;
//attribute highp mat4 modelMatrix;
//uniform mediump mat4 viewMatrix;
//uniform mediump mat4 projectionMatrix;
//uniform mediump mat4 modelMatrix;

void main( void )
{
	gl_Position = /*projectionMatrix * viewMatrix * modelMatrix */ vertex;
}