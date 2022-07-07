#ifndef RAYGEN_GEOMETRY_H_
#define RAYGEN_GEOMETRY_H_

#include <raylib.h>
#include <Eigen/Core>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Raygen {

enum ShaderMode { ShaderLambert, ShaderNormal };

Mesh gen_mesh(int v_num, float vertices[], int f_num, int faces[],
              const bool is_quad = false) {
  Mesh mesh = {0};

  int vertices_num = v_num / 3;
  int faces_num;

  std::vector<unsigned short> face_list;
  auto copy_face = [&](int _index_face_list, int _index_faces) {
    face_list[_index_face_list] = faces[_index_faces];
  };

  if (is_quad) {
    faces_num = f_num / 2;
    face_list.resize(faces_num * 3);
    for (int i = 0; i < f_num / 4; i++) {
      copy_face(i * 6, i * 4);
      copy_face(i * 6 + 1, i * 4 + 1);
      copy_face(i * 6 + 2, i * 4 + 2);
      copy_face(i * 6 + 3, i * 4);
      copy_face(i * 6 + 4, i * 4 + 2);
      copy_face(i * 6 + 5, i * 4 + 3);
    }
  } else {
    faces_num = f_num / 3;
    face_list.resize(faces_num * 3);
    for (int i = 0; i < f_num; i++) copy_face(i, i);
  }

  auto get_vertex = [&](const int _i) {
    return Eigen::Vector3f(vertices[_i * 3], vertices[_i * 3 + 1],
                           vertices[_i * 3 + 2]);
  };
  std::vector<Eigen::Vector3f> normal_vec3(vertices_num,
                                           Eigen::Vector3f::Zero());

  for (int i = 0; i < faces_num; i++) {
    int index1 = face_list[i * 3];
    int index2 = face_list[i * 3 + 1];
    int index3 = face_list[i * 3 + 2];
    Eigen::Vector3f vec1 = get_vertex(index1);
    Eigen::Vector3f vec2 = get_vertex(index2);
    Eigen::Vector3f vec3 = get_vertex(index3);
    Eigen::Vector3f normal = (vec2 - vec1).cross(vec3 - vec1);
    normal_vec3[index1] += normal;
    normal_vec3[index2] += normal;
    normal_vec3[index3] += normal;
  }

  std::vector<float> normals(v_num);
  for (int i = 0; i < vertices_num; i++) {
    normal_vec3[i].normalize();
    normals[i * 3] = normal_vec3[i](0);
    normals[i * 3 + 1] = normal_vec3[i](1);
    normals[i * 3 + 2] = normal_vec3[i](2);
  }

  mesh.vertices = (float *)RL_MALLOC(vertices_num * 3 * sizeof(float));
  memcpy(mesh.vertices, vertices, vertices_num * 3 * sizeof(float));
  // mesh.texcoords = (float *)RL_MALLOC(24 * 2 * sizeof(float));
  // memcpy(mesh.texcoords, texcoords, 24 * 2 * sizeof(float));
  mesh.normals = (float *)RL_MALLOC(vertices_num * 3 * sizeof(float));
  memcpy(mesh.normals, normals.data(), vertices_num * 3 * sizeof(float));
  mesh.indices =
      (unsigned short *)RL_MALLOC(faces_num * 3 * sizeof(unsigned short));
  memcpy(mesh.indices, face_list.data(),
         faces_num * 3 * sizeof(unsigned short));
  mesh.vertexCount = vertices_num;
  mesh.triangleCount = faces_num;
  UploadMesh(&mesh, false);
  return mesh;
}

void load_obj(std::vector<float> &vertices,
              std::vector<unsigned short> &indices, std::string filepath) {
  std::ifstream fin;
  fin.open(filepath);
  if (fin.fail()) {
    std::cout << "Fail Read File\n";
    return;
  }
  vertices.clear();
  vertices.reserve(256 * 16);
  indices.clear();
  indices.reserve(256 * 16);
  const int kBUFF_SIZE = 256;
  char buff[kBUFF_SIZE];
  while (fin.getline(buff, kBUFF_SIZE)) {
    if (buff[0] == '#') continue;
    if (buff[0] == 'v' && buff[1] == ' ') {
      char str[256];
      float x, y, z;
      std::istringstream is(buff);
      is >> str >> x >> y >> z;
      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);
    }
    if (buff[0] == 'f') {
      std::vector<std::string> vec_str;
      {
        std::istringstream iss(buff);
        std::string s;
        bool is_init = true;
        while (iss >> s) {
          if (is_init) {
            is_init = false;
            continue;
          }
          vec_str.push_back(s);
        }
      }
      std::vector<unsigned short> aI;
      aI.reserve(4);
      for (auto str : vec_str) {
        for (size_t i = 0; i < str.size(); ++i) {
          if (str[i] == '/') {
            str[i] = '\0';
            break;
          }
        }
        int i0 = std::stoi(str);
        aI.push_back(i0 - 1);
      }
      if (aI.size() == 3) {
        indices.push_back(aI[0]);
        indices.push_back(aI[1]);
        indices.push_back(aI[2]);
      }
      if (aI.size() == 4) {
        indices.push_back(aI[0]);
        indices.push_back(aI[1]);
        indices.push_back(aI[2]);
        indices.push_back(aI[0]);
        indices.push_back(aI[2]);
        indices.push_back(aI[3]);
      }
    }
  }
}

Shader gen_shader(ShaderMode shader_mode = ShaderNormal) {
  std::string vertex_shader =
      "#version 330\n"
      "in vec3 vertexPosition;\n"
      "in vec2 vertexTexCoord;\n"
      "in vec3 vertexNormal;\n"
      "in vec4 vertexColor;\n"
      "uniform mat4 mvp; \n"
      "uniform mat4 matModel; \n"
      "uniform mat4 matNormal; \n"
      "out vec3 fragPosition; \n"
      "out vec2 fragTexCoord; \n"
      "out vec4 fragColor; \n"
      "out vec3 fragNormal; \n"
      "void main()\n"
      "{\n"
      "    fragPosition = vec3(matModel*vec4(vertexPosition, 1.0));\n"
      "    fragTexCoord = vertexTexCoord;\n"
      "    fragColor = vertexColor;\n"
      "    fragNormal = normalize(vec3(matNormal*vec4(vertexNormal, 1.0)));\n"
      "    gl_Position = mvp*vec4(vertexPosition, 1.0);\n"
      "}\n";
  std::string frag_shader;
  switch (shader_mode) {
    case ShaderLambert:
      frag_shader = "";
      break;
    case ShaderNormal:
    default:
      frag_shader =
          "#version 330\n"
          "in vec3 fragNormal;\n"
          "out vec4 finalColor;\n"
          "void main()\n"
          "{\n"
          "    finalColor = vec4(abs(fragNormal.xyz), 1.0);\n"
          "}\n";
      break;
  }
  Shader shader = {0};
  shader = LoadShaderFromMemory(vertex_shader.c_str(), frag_shader.c_str());
  return shader;
}

}  // namespace Raygen

#endif  // RAYGEN_GEOMETRY_H_
