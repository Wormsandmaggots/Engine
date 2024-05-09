#version 330 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D ssaoInput;
uniform int rangeX, rangeY;
uniform float mul = 4;
uniform float texelSize;

void main()
{
    vec2 texelS = texelSize / vec2(textureSize(ssaoInput, 0));
    float result = 0.0;
    for (int x = -rangeX; x < rangeX; ++x)
    {
        for (int y = -rangeY; y < rangeY; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelS;
            result += texture(ssaoInput, TexCoords + offset).r;
        }
    }
    FragColor = result / (mul * mul);
    //FragColor = texture(ssaoInput, TexCoords).rgba;
    //FragColor = depthBuffer;
}
//Unknown4 May 2013 at 00:42
//
//I've added blur and have a very small problem.
//
//In this picture I've rendered with 128 samples a set of procedurally-generated sphere imposters, the problem I'm having is that around each sphere there is a thin halo of non-occlusion:
//
//http://troll.ws/image/f57a6a71
//
//Do you have any idea/suggestion about what causes this issue and how to solve this problem?
//
//The shader I'm using are in this directory: https://github.com/chemlab/chemlab/tree/master/chemlab/graphics/postprocessing/shaders
//John Chapman5 May 2013 at 14:28
//
//This is the main issue with indiscriminately blurring the AO result. Areas of occlusion/non-occlusion will tend to 'leak' - most noticeably where there are sharp discontinuities in the depth buffer.
//
//The solution is to use a more complex blur which samples the depth buffer and only blurs pixels which are at a similar depth.
//
//Another option might be to simply dilate the AO result slightly (after applying the blur). I'm not sure how well this will work, though.