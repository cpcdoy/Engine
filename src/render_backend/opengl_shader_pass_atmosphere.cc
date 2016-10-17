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

    resource::obj_loader o;
    o.load_obj("res/obj/skydome.obj");

    sky_dome = std::make_shared<resource::gl_mesh>(o.generate_mesh());

    debug::log::get(debug::logINFO) << "Generating the skydome's VAO" << std::endl;

    GLuint vao;
    GLuint vertices_vbo, uvs_vbo, normals_vbo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Vertices
    glGenBuffers(1, &vertices_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
    glBufferData(GL_ARRAY_BUFFER, sky_dome->get_vertices().size() * sizeof(glm::vec3), &sky_dome->get_vertices()[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    //UVs
    glGenBuffers(1, &uvs_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_vbo);
    glBufferData(GL_ARRAY_BUFFER, sky_dome->get_uvs().size() * sizeof(glm::vec3), &sky_dome->get_uvs()[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);

    //Normals
    glGenBuffers(1, &normals_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sky_dome->get_normals().size() * sizeof(glm::vec3), &sky_dome->get_normals()[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    glBindVertexArray(0);

    debug::log::get(debug::logINDENT, 5) << "VAO : " << vao << std::endl;

    sky_dome->set_vao(vao);
    sky_dome->set_scale(glm::vec3(30));
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

    glUniform3fv(uniforms[3], 1, &(cam->get_camera_position())[0]);
    glUniform3fv(uniforms[4], 1, &(cam->get_camera_position())[0]);

    auto camera_height = cam->get_camera_position().length();
    glUniform1i(uniforms[5], camera_height);
    glUniform1i(uniforms[6], camera_height * camera_height);

    glBindVertexArray(m->get_vao());
    glDrawArrays(GL_TRIANGLES, 0, m->get_vertices().size());
  }
}
