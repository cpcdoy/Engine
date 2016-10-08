#pragma once

# include "../../3rd_party/PEGTL/pegtl.hh"

namespace render_backend
{
    // Separators
    struct sep
        : pegtl::star<pegtl::space> {};

    struct plus_sep
        : pegtl::plus<pegtl::space> {};

    struct inline_comment
        : pegtl::if_must<pegtl_string_t("//"),
                         pegtl::until<pegtl::eol>> {};

    struct ignored
        : pegtl::sor<plus_sep,
                     inline_comment> {};
    //

    // Misc
    struct digits
        : pegtl::plus<pegtl::digit> {};

    struct semicol
        : pegtl::one<';'> {};
    //

    // Backend version
    struct backend_version
        : digits {};

    struct backend_profile
        : pegtl::sor<pegtl_string_t("core"),
                     pegtl_string_t("compatibility")> {};

    struct backend_conf
        : pegtl::seq<backend_version,
                     sep,
                     pegtl::opt<backend_profile>> {};

    struct version
        : pegtl::seq<pegtl_string_t("backend"),
                     sep,
                     pegtl_string_t("version"),
                     sep,
                     backend_conf> {};
    //

    // Vardec
    struct vardec_name
        : pegtl::seq<pegtl::plus<pegtl::alpha>,
                     pegtl::star<pegtl::alnum>> {};

    struct vardec_value_int
        : digits {};

    struct vardec_value_float
        : pegtl::seq<digits,
                     pegtl::one<'.'>,
                     digits,
                     pegtl::opt<pegtl::one<'f'>>> {};

    struct vardec_init
        : pegtl::sor<vardec_value_float, vardec_value_int> {};

    struct vardec
        : pegtl::seq<pegtl_string_t("var"),
                     sep,
                     vardec_name,
                     sep,
                     pegtl::sor<semicol,
                                pegtl::seq<pegtl::one<'='>,
                                           sep,
                                           vardec_init,
                                           sep,
                                           semicol>>> {};
    //

    // Rules and errors
    struct rule_list
        : pegtl::until<pegtl::eof,
                       pegtl::sor<ignored,
                                 vardec,
                                 version>> {};

    struct error_eater;

    struct allowed_rules
        : pegtl::until<pegtl::eof,
                       pegtl::sor<ignored,
                                  vardec,
                                  version,
                                  error_eater>> {};

    struct error_eater
        : pegtl::seq<pegtl::opt<pegtl::raise<rule_list>>,
                     pegtl::until<pegtl::at<allowed_rules>, pegtl::any>> {};
    //
}
