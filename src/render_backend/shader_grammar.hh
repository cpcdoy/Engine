#pragma once

# include "../../3rd_party/PEGTL/pegtl.hh"

namespace render_backend
{
    struct inline_comment
        : pegtl::if_must<pegtl_string_t("//"), pegtl::until<pegtl::eof>> {};

    struct rule_list
        : pegtl::until<
          pegtl::eof,
          pegtl::sor<inline_comment>> {};
}
