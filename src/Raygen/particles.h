#ifndef RAYGEN_PARTICLES_H_
#define RAYGEN_PARTICLES_H_

#include <raylib.h>

namespace Raygen {

class ParticleSystem {
public:
    ParticleSystem(const int num, Mesh mesh, Color color, bool lighting = true) : m_num(num), m_mesh(mesh)
    {
        std::string vertex_shader, frag_shader;
        // if (lighting) {
        vertex_shader = "#version 330\n"
                        "in vec3 vertexPosition;\n"
                        "in vec2 vertexTexCoord;\n"
                        "in vec3 vertexNormal;\n"
                        "in vec4 vertexColor;\n"
                        "in mat4 instanceTransform;\n"
                        "uniform mat4 mvp;\n"
                        "uniform mat4 matNormal;\n"
                        "out vec3 fragPosition;\n"
                        "out vec2 fragTexCoord;\n"
                        "out vec4 fragColor;\n"
                        "out vec3 fragNormal;\n"
                        "void main()\n"
                        "{\n"
                        "    mat4 mvpi = mvp*instanceTransform;\n"
                        "    fragPosition = vec3(mvpi*vec4(vertexPosition, 1.0));\n"
                        "    fragTexCoord = vertexTexCoord;\n"
                        "    fragColor = vertexColor;\n"
                        "    fragNormal = normalize(vec3(matNormal*vec4(vertexNormal, 1.0)));\n"
                        "    gl_Position = mvpi*vec4(vertexPosition, 1.0);\n"
                        "}";
        frag_shader   = "#version 330\n"
                        "in vec2 fragTexCoord;\n"
                        "in vec4 fragColor;\n"
                        "in vec3 fragPosition;\n"
                        "in vec3 fragNormal;\n"
                        "uniform sampler2D texture0;\n"
                        "uniform vec4 colDiffuse;\n"
                        "out vec4 finalColor;\n"
                        "uniform vec3 viewPos;\n"
                        "void main()\n"
                        "{\n"
                        "    vec4 texelColor = colDiffuse * fragColor;\n"
                        "    vec3 normal = normalize(fragNormal) * 0.9;\n"
                        "    vec3 viewD = normalize(viewPos - fragPosition);\n"
                        "    vec3 specular = vec3(1.0);\n"
                        "    float NdotL = max(dot(normal, viewD), 0.0);\n"
                        "    vec3 lightDot = vec3(NdotL);\n"
                        "    float specCo = 0.0;\n"
                        "    if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewD, reflect(-(-viewD), normal))), 16.0); \n"
                        "    specular += specCo;\n"
                        "    finalColor = (texelColor*((colDiffuse + vec4(specular,1))*vec4(lightDot, 1.0)));\n"
                        "}";
        //}

        m_shader                               = LoadShaderFromMemory(vertex_shader.c_str(), frag_shader.c_str());
        m_shader.locs[SHADER_LOC_MATRIX_MVP]   = GetShaderLocation(m_shader, "mvp");
        m_shader.locs[SHADER_LOC_VECTOR_VIEW]  = GetShaderLocation(m_shader, "viewPos");
        m_shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(m_shader, "instanceTransform");

        m_material                                  = LoadMaterialDefault();
        m_material.shader                           = m_shader;
        m_material.maps[MATERIAL_MAP_DIFFUSE].color = color;
    }

    void update_camera(const Camera& camera)
    {
        float cameraPos[3] = {camera.position.x, camera.position.y, camera.position.z};
        SetShaderValue(m_shader, m_shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
    }

    void draw_instances(Matrix* transforms) { DrawMeshInstanced(m_mesh, m_material, transforms, m_num); }

public:
    const int m_num;
    Mesh      m_mesh;
    Shader    m_shader;
    Material  m_material;
};

} // namespace Raygen

#endif // RAYGEN_PARTICLES_H_
