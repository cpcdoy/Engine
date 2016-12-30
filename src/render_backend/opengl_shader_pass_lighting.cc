#include "opengl_shader_pass_lighting.hh"

namespace render_backend
{
  opengl_shader_pass_lighting::opengl_shader_pass_lighting(std::string vs, std::string fs)
  : opengl_shader_pass(vs, fs)
  {
    uniforms.push_back(glGetUniformLocation(program, "view")); // 0
    uniforms.push_back(glGetUniformLocation(program, "projection"));
    uniforms.push_back(glGetUniformLocation(program, "light_pos"));
    uniforms.push_back(glGetUniformLocation(program, "view_pos"));
    uniforms.push_back(glGetUniformLocation(program, "light_space_matrix"));
    uniforms.push_back(glGetUniformLocation(program, "shadow_map")); // 5
    uniforms.push_back(glGetUniformLocation(program, "ao_map"));
    uniforms.push_back(glGetUniformLocation(program, "diffuse_map"));
    uniforms.push_back(glGetUniformLocation(program, "metalness_roughness_baked_ao_map"));
    //uniforms.push_back(glGetUniformLocation(program, "roughness_map"));
    //uniforms.push_back(glGetUniformLocation(program, "baked_ao_map"));
    uniforms.push_back(glGetUniformLocation(program, "model")); // 9
    uniforms.push_back(glGetUniformLocation(program, "trans_inv_model"));
    uniforms.push_back(glGetUniformLocation(program, "screen_res")); // 11
    uniforms.push_back(glGetUniformLocation(program, "g_position_depth"));
    uniforms.push_back(glGetUniformLocation(program, "g_normal")); // 13

    static const GLfloat quad_vertex_buffer[] =
    {
      -1.0f, 1.0f, 0.0f,
      -1.0f, -1.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      1.0f, -1.0f, 0.0f,
    };

    static const GLfloat quad_uvs[] =
    {
      0.0f, 1.0f,
      0.0f, 0.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
    };

    GLuint quad_vertex_vbo, quad_uv_vbo;
    //quad VAO
    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);
    glGenBuffers(1, &quad_vertex_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertex_buffer), quad_vertex_buffer, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glGenBuffers(1, &quad_uv_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_uv_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_uvs), &quad_uvs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);

    glUseProgram(program);

    glUniform1i(uniforms[5], 0);
    glUniform1i(uniforms[6], 1);
    glUniform1i(uniforms[7], 2);
    glUniform1i(uniforms[8], 3);

    glUniform2fv(uniforms[11], 1, &glm::vec2(w, h)[0]);
    glUniform1i(uniforms[12], 4);
    glUniform1i(uniforms[13], 5);
  }

  void opengl_shader_pass_lighting::process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>&, std::shared_ptr<scene::camera> cam, long)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    auto projection = cam->get_projection_matrix();
    auto view = cam->get_view_matrix();

    //
    glm::vec3 lightPos = glm::vec3(-5, 11, -5);
    lightPos.z = cos(glfwGetTime()) * 2.0f;

    float area = 10;
    glm::mat4 depth_proj = glm::ortho(-area, area, -area, area, -10.0f, 100.0f);
    glm::mat4 depth_view = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0, 1, 0));

    glm::mat4 depth_VP = depth_proj * depth_view;
    //

    glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &projection[0][0]);
    // Set light uniforms
    glUniform3fv(uniforms[2], 1, &lightPos[0]);
    glUniform3fv(uniforms[3], 1, &cam->get_camera_position()[0]);
    glUniformMatrix4fv(uniforms[4], 1, GL_FALSE, &depth_VP[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, opengl_pipeline_state::instance().get_state_of("depth_texture"));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, opengl_pipeline_state::instance().get_state_of("ssao_texture"));

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, opengl_pipeline_state::instance().get_state_of("g_albedo"));

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, opengl_pipeline_state::instance().get_state_of("g_metalness_roughness_baked_ao"));

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, opengl_pipeline_state::instance().get_state_of("g_position_depth"));

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, opengl_pipeline_state::instance().get_state_of("g_normal"));

    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

  void opengl_shader_pass_lighting::operator()(const event::performance_statistics_event&)
  {
  }
}
