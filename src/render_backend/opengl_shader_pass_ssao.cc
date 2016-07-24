#include "opengl_shader_pass_ssao.hh"

namespace render_backend
{
  opengl_shader_pass_ssao::opengl_shader_pass_ssao(std::string vs, std::string fs)
    : opengl_shader_pass(vs, fs)
  {
    std::uniform_real_distribution<GLfloat> random_floats(0.0, 1.0);
    std::default_random_engine generator;
    for (GLuint i = 0; i < 32; ++i)
    {
      glm::vec3 sample(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, random_floats(generator));
      sample = glm::normalize(sample);
      sample *= random_floats(generator);
      GLfloat scale = GLfloat(i) / 32.0;

      scale = lerp(0.1f, 1.0f, scale * scale);
      sample *= scale;
      ssao_kernel.push_back(sample);
    }

    for (GLuint i = 0; i < 16; i++)
    {
      glm::vec3 noise(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, 0.0f);
      ssao_noise.push_back(noise);
    }

    glGenRenderbuffers(1, &rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
    // - Finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      debug::log::get(debug::log_level::logERROR) << "Depth Framebuffer not complete" << std::endl;

    //Generate noise texture
    glGenTextures(1, &noise_texture);
    glBindTexture(GL_TEXTURE_2D, noise_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssao_noise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenFramebuffers(1, &ssao_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo);

    GLuint ssao_texture;
    // - SSAO color buffer
    glGenTextures(1, &ssao_texture);
    glBindTexture(GL_TEXTURE_2D, ssao_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_texture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      debug::log::get(debug::log_level::logERROR) << "SSAO Framebuffer not complete" << std::endl;

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

    uniforms.push_back(glGetUniformLocation(program, "projection"));
    uniforms.push_back(glGetUniformLocation(program, "samples"));
    uniforms.push_back(glGetUniformLocation(program, "inv_proj"));

		for (GLuint i = 0; i < 32; ++i)
			glUniform3fv(glGetUniformLocation(program, ("samples[" + std::to_string(i) + "]").c_str()), 1, &ssao_kernel[i][0]);

    opengl_pipeline_state::instance().add_state("ssao_texture", ssao_texture);

    glUniform1i(glGetUniformLocation(program, "g_position_depth"), 0);
    glUniform1i(glGetUniformLocation(program, "g_normal"), 1);
    glUniform1i(glGetUniformLocation(program, "tex_noise"), 2);

    uniforms.push_back(glGetUniformLocation(program, "screen_res"));
    glUniform2fv(uniforms.back(), 1, &glm::vec2(w, h)[0]);
  }

  void opengl_shader_pass_ssao::process_pass(std::vector<std::shared_ptr<resource::gl_mesh>>&, std::shared_ptr<scene::camera> cam)
  {
		glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, opengl_pipeline_state::instance().get_state_of("g_position_depth"));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, opengl_pipeline_state::instance().get_state_of("g_normal"));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noise_texture);

    glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &cam->get_projection_matrix()[0][0]);
    //glUniformMatrix4fv(uniforms[2], 1, GL_FALSE, &glm::inverse(cam->get_projection_matrix())[0][0]);

    glBindVertexArray(quad_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}
