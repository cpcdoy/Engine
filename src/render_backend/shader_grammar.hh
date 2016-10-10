#pragma once

# include "../../3rd_party/PEGTL/pegtl.hh"

namespace render_backend
{
    // Misc
    template<typename T>
    struct must
        : pegtl::must<T> {};

    struct digits
        : pegtl::plus<pegtl::digit> {};

    struct semicol
        : pegtl::one<';'> {};
    //

    // Separators
    struct sep
        : pegtl::star<pegtl::space> {};

    struct plus_sep
        : pegtl::plus<pegtl::space> {};

    struct comment_sec_delim
        : pegtl::one<'/'> {};

    struct comment_delim
        : pegtl::if_must<pegtl::one<'/'>,
                         must<comment_sec_delim>> {};

    struct inline_comment
        : pegtl::if_must<comment_delim,
                         pegtl::until<pegtl::eol>> {};

    struct ignored
        : pegtl::sor<plus_sep,
                     inline_comment> {};
    //

    // Backend version
    struct backend_version
        : digits {};

    struct backend_profile
        : pegtl::sor<pegtl_string_t("core"),
                     pegtl_string_t("compatibility")> {};

    struct backend_conf
        : pegtl::seq<must<backend_version>,
                     sep,
                     pegtl::opt<backend_profile>> {};

    struct version
        : pegtl::seq<pegtl_string_t("backend"),
                     sep,
                     pegtl_string_t("version"),
                     sep,
                     backend_conf> {};
    //

    // Operators
    struct op_plus
        : pegtl::one<'+'> {};

    struct op_minus
        : pegtl::one<'-'> {};

    struct op_mul
        : pegtl::one<'*'> {};

    struct op_div
        : pegtl::one<'/'> {};

    struct op_eq
        : pegtl::one<'='> {};

    struct op_neq
        : pegtl_string_t("!=") {};

    struct op_less
        : pegtl::one<'<'> {};

    struct op_greater
        : pegtl::one<'>'> {};

    struct op_geq
        : pegtl_string_t(">=") {};

    struct op_leq
        : pegtl_string_t("<=") {};

    struct op_land
        : pegtl::one<'&'> {};

    struct op_lor
        : pegtl::one<'|'> {};

    struct op_and
        : pegtl_string_t("&&") {};

    struct op_or
        : pegtl_string_t("||") {};

    struct op
        : pegtl::sor<op_mul,
                     op_div,
                     op_plus,
                     op_minus,
                     op_geq,
                     op_leq,
                     op_eq,
                     op_neq,
                     op_less,
                     op_greater,
                     op_and,
                     op_or,
                     op_land,
                     op_lor> {};

    struct exp;

    struct op_apply
        : pegtl::seq<exp,
                     sep,
                     op,
                     sep,
                     exp> {};

    // Vardec
    struct vardec_name
        : pegtl::seq<pegtl::plus<pegtl::alpha>,
                     pegtl::star<pegtl::seq<pegtl::star<pegtl::one<'_'>>,
                                                        pegtl::alnum>>> {};

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
                     must<vardec_name>,
                     sep,
                     pegtl::opt<pegtl::seq<pegtl::one<'='>,
                                           sep,
                                           must<vardec_init>>>> {};
    //

    // Rules and errors
    // Specials
    struct specials
        : pegtl::sor<version> {};

    // Exp
    struct exp
        : pegtl::sor<digits> {};

    struct exp_construct
        : pegtl::seq<exp, sep, must<semicol>> {};

    struct exps
        : pegtl::plus<pegtl::seq<exp_construct, pegtl::star<exp_construct>>> {};

    // Dec
    struct dec
        : pegtl::sor<vardec> {};

    struct dec_construct
        : pegtl::seq<dec, sep, must<semicol>> {};

    struct decs
        : pegtl::plus<pegtl::seq<dec_construct, pegtl::star<dec_construct>>> {};

    // Rules
    struct rules
        : pegtl::sor<ignored,
                     specials,
                     exps,
                     decs> {};

    struct rule_list
        : pegtl::until<pegtl::eof,
                       must<rules>> {};

    // Errors
    struct error_eater;

    struct allowed_rules
        : pegtl::until<pegtl::eof,
                       pegtl::sor<ignored,
                                  specials,
                                  exps,
                                  decs,
                                  error_eater>> {};

    struct error_eater
        : pegtl::seq<pegtl::opt<pegtl::raise<rule_list>>,
                     pegtl::until<pegtl::at<allowed_rules>, pegtl::any>> {};
    //
}
