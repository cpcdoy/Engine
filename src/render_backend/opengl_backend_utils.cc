#include "opengl_backend.hh"
#include "opengl_pipeline_state.hh"

namespace render_backend
{
  std::vector<std::string> opengl_backend::get_gl_extensions()
  {
    std::vector<std::string> caps;

    caps.push_back("GL_ARB_multi_draw_indirect");
    caps.push_back("GL_ARB_gpu_shader5");
    caps.push_back("GL_ARB_bindless_texture");

    return caps;
  }

  bool opengl_backend::check_gl_extensions()
  {
    const char* extensions = (const char*)glGetString(GL_EXTENSIONS);

    int max_vaos;
    int max_layers;
    float anisotropy_level = 0.0f;
    int max_patch_verts = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vaos);
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &max_patch_verts);
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy_level);
    debug::log::get(debug::logDEBUG) << "Max VAOs number : " << max_vaos << std::endl;
    debug::log::get(debug::logDEBUG) << "Max texture2D layers : " << max_layers << std::endl;
    debug::log::get(debug::logDEBUG) << "Max patch vertices : " << max_patch_verts << std::endl;
    debug::log::get(debug::logDEBUG) << "Max anisotropy level : " << anisotropy_level << std::endl;
    opengl_pipeline_state::instance().add_state_lock("aniso_level", anisotropy_level);
    opengl_pipeline_state::instance().add_state_lock("max_patches", max_patch_verts);

    if (extensions)
      debug::log::get(debug::logINFO) << "GL_EXTENSIONS : " << extensions << std::endl;

    gl_caps.push_back((const char*)glGetString(GL_VENDOR));
    debug::log::get(debug::logINFO) << "OpenGL vendor: " << std::endl;
    debug::log::get(debug::logINDENT, 5) << gl_caps.back() << std::endl;
    gl_caps.push_back((const char*)glGetString(GL_RENDERER));
    debug::log::get(debug::logINFO) << "OpenGL renderer: " << std::endl;
    debug::log::get(debug::logINDENT, 5) << gl_caps.back() << std::endl;
    gl_caps.push_back((const char*)glGetString(GL_VERSION));
    debug::log::get(debug::logINFO) << "OpenGL version: " << std::endl;
    debug::log::get(debug::logINDENT, 5) << gl_caps.back() << std::endl;
    gl_caps.push_back(std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
    debug::log::get(debug::logINFO) << "OpenGL GLSL version: " << std::endl;
    debug::log::get(debug::logINDENT, 5) << gl_caps.back() << std::endl;

    GLint num_extensions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);

    debug::log::get(debug::logINFO) << "Supported extensions (" << num_extensions << ") :"
                                    << std::endl;
    auto caps = get_gl_extensions();
    for (GLint i = 0; i < num_extensions; ++i)
    {
      gl_caps.push_back(std::string((const char*)glGetStringi(GL_EXTENSIONS, i)));
      debug::log::get(debug::logINDENT, 5) << gl_caps.back() << std::endl;
    }

    for (auto i = caps.begin(); i != caps.end(); i++)
    {
      auto r = std::find(gl_caps.begin(), gl_caps.end(), *i);
      if (r == gl_caps.end())
        debug::log::get(debug::logWARN, 5) << "Unsupported " << *i << std::endl;
    }

    return true;
  }

  float lerp(float v0, float v1, float t)
  {
    return (1 - t) * v0 + t * v1;
  }
}
