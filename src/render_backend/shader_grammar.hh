#pragma once

# include "../../3rd_party/PEGTL/pegtl.hh"

namespace render_backend
{
    struct inline_comment
        : pegtl::if_must<pegtl_string_t("//"),
            pegtl::until<pegtl::eof>> {};

    struct rule_list
        : pegtl::until<pegtl::eof,
                       pegtl::sor<inline_comment>> {};

    struct error_eater;

    struct allowed_rules
        : pegtl::until<pegtl::eof,
                       pegtl::sor<inline_comment, error_eater>> {};

    struct error_eater
        : pegtl::seq<pegtl::opt<pegtl::raise<rule_list>>,
                     pegtl::until<pegtl::at<allowed_rules>, pegtl::any>> {};
}
