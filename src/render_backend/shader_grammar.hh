#pragma once

# include "../../3rd_party/PEGTL/pegtl.hh"

namespace render_backend
{
    struct sep
        : pegtl::star<pegtl::space> {};

    struct plus_sep
        : pegtl::plus<pegtl::space> {};

    struct inline_comment
        : pegtl::if_must<pegtl_string_t("//"),
                         pegtl::until<pegtl::eol>> {};

    struct backend_version
        : pegtl::digit {};

    struct backend_profile
        : pegtl::sor<pegtl_string_t("core"),
                     pegtl_string_t("compatibility")> {};

    struct backend_conf
        : pegtl::seq<backend_version,
                     sep,
                     pegtl::opt<backend_profile>> {};

    struct version
        : pegtl::seq<pegtl_string_t("backend version"),
                     sep,
                     backend_conf> {};

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
