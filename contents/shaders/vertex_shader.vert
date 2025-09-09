#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

out vec3 ourColor; // output a color to the fragment shader
uniform float h_offset;

void main()
{
    gl_Position = vec4(aPos, 1.0);

    // The output of our fragment's color is equal to the (interpolated) coordinate of
    // the triangle. What is the coordinate of the bottom-left point of our triangle? 
    // This is (-0.5f, -0.5f, 0.0f). Since the xy values are negative they are clamped to 
    // a value of 0.0f. This happens all the way to the center sides of the triangle since 
    // from that point on the values will be interpolated positively again. Values of 0.0f 
    // are of course black and that explains the black side of the triangle.
    ourColor = aPos; // set ourColor to the input color we got from the vertex data
}    