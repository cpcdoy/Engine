#include "opengl_shader_pass_atmosphere.hh"
#include "../resource/obj_loader.hh"

namespace render_backend
{
  opengl_shader_pass_atmosphere::opengl_shader_pass_atmosphere(std::string vs, std::string fs)
    : opengl_shader_pass(vs, fs)
  {
    uniforms.push_back(glGetUniformLocation(program, "view")); // 0
    uniforms.push_back(glGetUniformLocation(program, "projection"));
    uniforms.push_back(glGetUniformLocation(program, "model"));
    uniforms.push_back(glGetUniformLocation(program, "v3CameraPos"));
    uniforms.push_back(glGetUniformLocation(program, "v3LightPos"));
    uniforms.push_back(glGetUniformLocation(program, "fCameraHeight")); // 5
    uniforms.push_back(glGetUniformLocation(program, "fCameraHeight2"));

    debug::log::get(debug::logINFO) << "Generating the skydome's VAO" << std::endl;

    GLuint vao;
    GLuint vertices_vbo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    auto skydome = create_skydome_with_atmosphere(10.0, 10.25);

    //Vertices
    glGenBuffers(1, &vertices_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
    glBufferData(GL_ARRAY_BUFFER, skydome.size() * sizeof(glm::vec3), &skydome[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    glBindVertexArray(0);

    debug::log::get(debug::logINDENT, 5) << "VAO : " << vao << std::endl;

    resource::obj_loader o;
    o.load_obj("res/obj/skydome_atmo.obj");

    sky_dome = std::make_shared<resource::gl_mesh>(o.generate_mesh());
    sky_dome->set_vertices(skydome);
    sky_dome->set_vao(vao);
    sky_dome->set_pos(glm::vec3(0, 4, 0));
  }

  std::vector<glm::vec3> opengl_shader_pass_atmosphere::create_skydome_with_atmosphere(float r_inner, float r_outer)
  {
    const int numX = 16;
    const int numY = 4;

    const int num_vertices = (2 * numY + 1) * numX * 3; // triangles * 3

    std::vector<glm::vec3> vertices;
    vertices.reserve(num_vertices);
    glm::vec3 va, vb, vc, vd;

    const float PI = 3.1415926535897932;
    float stepa  = PI * 2.0f / numX;
    float startb = asin(r_inner / r_outer), stepb = (PI / 2.0f - startb) / 4.0f;

    for (int y = 0; y < numY; y++)
    {
      float b = startb + stepb * y;

      for (int x = 0; x < numX; x++)
      {
        float a = stepa * x;

        va = glm::vec3( sinf(a)         * cosf(b) * r_outer,         sinf(b) * r_outer,         -cosf(a)         * cosf(b) * r_outer  );
        vb = glm::vec3( sinf(a + stepa) * cosf(b) * r_outer,         sinf(b) * r_outer,         -cosf(a + stepa) * cosf(b) * r_outer  );
        vc = glm::vec3( sinf(a + stepa) * cosf(b + stepb) * r_outer, sinf(b + stepb) * r_outer, -cosf(a + stepa) * cosf(b + stepb) * r_outer  );
        vd = glm::vec3( sinf(a)         * cosf(b + stepb) * r_outer, sinf(b + stepb) * r_outer, -cosf(a)         * cosf(b + stepb) * r_outer  );

        vertices.push_back(va);
        vertices.push_back(vb);
        vertices.push_back(vc);

        vertices.push_back(vc);
        vertices.push_back(vd);
        vertices.push_back(va);
      }
    }

    float b = startb + stepb * 3;

    for (int x = 0; x < numX; x++)
    {
      float a = stepa * x;

      va = glm::vec3 ( sinf(a)         * cosf(b) * r_outer, sinf(b) * r_outer, -cosf(a)         * cosf(b) * r_outer  );
      vb = glm::vec3 ( sinf(a + stepa) * cosf(b) * r_outer, sinf(b) * r_outer, -cosf(a + stepa) * cosf(b) * r_outer  );
      vc = glm::vec3 ( 0.0f, r_outer, 0.0f  );

      vertices.push_back(va);
      vertices.push_back(vb);
      vertices.push_back(vc);

    }

    debug::log::get(debug::logINDENT, 5) << "vec len " << vertices.size() << " numX " << numX << " " << numY << std::endl;

    return vertices;
  }

  void opengl_shader_pass_atmosphere::process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>& render_queue, std::shared_ptr<scene::camera> cam, long rq_size)
  {
    auto m = sky_dome;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(program);

    auto projection = cam->get_projection_matrix();
    auto view = cam->get_view_matrix();

    //
    glm::vec3 lightPos = glm::vec3(-5, 11, -5);
    lightPos.z = cos(glfwGetTime()) * 2.0f;

    glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(uniforms[2], 1, GL_FALSE, &m->get_model()[0][0]);

    glUniform3f(uniforms[3], 0, 1, 0);
    glUniform3fv(uniforms[4], 1, &lightPos[0]);

    auto camera_height = cam->get_camera_position().length();
    glUniform1i(uniforms[5], 1);
    glUniform1i(uniforms[6], 1);

    glBindVertexArray(m->get_vao());
    glDrawArrays(GL_TRIANGLES, 0, m->get_vertices().size());
  }
}
