/*

Copyright (C) 2014 Floyd Mulenga Chitalu

Permission is hereby granted, free of charge, to obtain a copy
of this software, to deal in the Software without restriction, including
without limitation the rights to [use], [copy], [modify], [merge], [publish],
[distribute], [sublicense], and/or [sell] copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM(S), DAMAGE(S) OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <cprintf/cprintf.h>

//#include <algorithm>
//#include <clocale>
//#include <cwctype>
//#include <memory>
//#include <type_traits>

//#include <cstdint>

#include <cstdio>
#include <functional> //std::function
#include <stdlib.h>   /* atoi */
#include <wchar.h>    //wcstol

#if _WIN32
#include <Windows.h>

HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE stderr_handle = GetStdHandle(STD_ERROR_HANDLE);

#define CONFIG_FG (0)
#define CONFIG_BG (1)
#define CONFIG_FGBG (2)

#endif // _WIN32

#define _CPF_TOKEN_PREFIX "$"

#define __CPRINTF_capi_Def0(_formattype, _argtype)                             \
  __CPRINTF_capi_Sig0(_formattype, _argtype) \
{                              \
    switch (stream) {                                                          \
    case 1:                                                                    \
      return cprintf(stdout, format, arg);                                     \
      break;                                                                   \
    case 2:                                                                    \
      return cprintf(stderr, format, arg);                                     \
      break;                                                                   \
    default:                                                                   \
      return 1;                                                                \
    }                                                                          \
  \
}

#define __CPRINTF_capi_Def1(_formattype)                                       \
  \
__CPRINTF_capi_Def0(_formattype, c_int32);                                     \
  \
__CPRINTF_capi_Def0(_formattype, c_long);                                      \
  \
__CPRINTF_capi_Def0(_formattype, c_float);                                     \
  \
__CPRINTF_capi_Def0(_formattype, c_char_p);                                    \
  \
__CPRINTF_capi_Def0(_formattype, c_wchar_p);

__CPRINTF_capi_Def1(c_char_p) __CPRINTF_capi_Def1(c_wchar_p)

    namespace _cprintf_ {
  CPF_API const _cprintf_::unicode_string_vector_t _cprintf_::std_tokens = {
    L"r",
    L"g",
    L"b",
    L"y",
    L"m",
    L"c",
    L"w",
    L"r#",
    L"g#",
    L"b#",
    L"y#",
    L"m#",
    L"c#",
    L"w#",
    L"R",
    L"G",
    L"B",
    L"Y",
    L"M",
    L"C",
    L"W",
    L"R#",
    L"G#",
    L"B#",
    L"Y#",
    L"M#",
    L"C#",
    L"W#",
    L"rr",
    L"rb",
    L"rg",
    L"ry",
    L"rm",
    L"rc",
    L"rw",
    L"gg",
    L"gb",
    L"gr",
    L"gy",
    L"gm",
    L"gc",
    L"gw",
    L"bb",
    L"br",
    L"bg",
    L"by",
    L"bm",
    L"bc",
    L"bw",
    L"yy",
    L"yb",
    L"yg",
    L"yr",
    L"ym",
    L"yc",
    L"yw",
    L"mm",
    L"mr",
    L"mg",
    L"my",
    L"mb",
    L"mc",
    L"mw",
    L"cc",
    L"cr",
    L"cg",
    L"cy",
    L"cm",
    L"cb",
    L"cw",
    L"ww",
    L"wr",
    L"wg",
    L"wy",
    L"wm",
    L"wc",
    L"wb",
    L"Rr",
    L"Rb",
    L"Rg",
    L"Ry",
    L"Rm",
    L"Rc",
    L"Rw",
    L"Gg",
    L"Gb",
    L"Gr",
    L"Gy",
    L"Gm",
    L"Gc",
    L"Gw",
    L"Bb",
    L"Br",
    L"Bg",
    L"By",
    L"Bm",
    L"Bc",
    L"Bw",
    L"Yy",
    L"Yb",
    L"Yg",
    L"Yr",
    L"Ym",
    L"Yc",
    L"Yw",
    L"Mm",
    L"Mr",
    L"Mg",
    L"My",
    L"Mb",
    L"Mc",
    L"Mw",
    L"Cc",
    L"Cr",
    L"Cg",
    L"Cy",
    L"Cm",
    L"Cb",
    L"Cw",
    L"Ww",
    L"Wr",
    L"Wg",
    L"Wy",
    L"Wm",
    L"Wc",
    L"Wb",
    L"rR",
    L"rB",
    L"rG",
    L"rY",
    L"rM",
    L"rC",
    L"rW",
    L"gG",
    L"gB",
    L"gR",
    L"gY",
    L"gM",
    L"gC",
    L"gW",
    L"bB",
    L"bR",
    L"bG",
    L"bY",
    L"bM",
    L"bC",
    L"bW",
    L"yY",
    L"yB",
    L"yG",
    L"yR",
    L"yM",
    L"yC",
    L"yW",
    L"mM",
    L"mR",
    L"mG",
    L"mY",
    L"mB",
    L"mC",
    L"mW",
    L"cC",
    L"cR",
    L"cG",
    L"cY",
    L"cM",
    L"cB",
    L"cW",
    L"wW",
    L"wR",
    L"wG",
    L"wY",
    L"wM",
    L"wC",
    L"wB",
    L"RR",
    L"RB",
    L"RG",
    L"RY",
    L"RM",
    L"RC",
    L"RW",
    L"GG",
    L"GB",
    L"GR",
    L"GY",
    L"GM",
    L"GC",
    L"GW",
    L"BB",
    L"BR",
    L"BG",
    L"BY",
    L"BM",
    L"BC",
    L"BW",
    L"YY",
    L"YB",
    L"YG",
    L"YR",
    L"YM",
    L"YC",
    L"YW",
    L"MM",
    L"MR",
    L"MG",
    L"MY",
    L"MB",
    L"MC",
    L"MW",
    L"CC",
    L"CR",
    L"CG",
    L"CY",
    L"CM",
    L"CB",
    L"CW",
    L"WW",
    L"WR",
    L"WG",
    L"WY",
    L"WM",
    L"WC",
    L"WB"

#if !defined(CPF_WINDOWS_BUILD)
    // bold, dim, blink, reverse, hidden
    ,
    L"bld",
    L"dim",
    L"uln",
    L"blk",
    L"rvs",
    L"hid",
    L"?bld",
    L"?dim",
    L"?uln",
    L"?blk",
    L"?rvs",
    L"?hid",
#endif // #if !defined(CPF_WINDOWS_BUILD)
  };

#if defined(CPF_WINDOWS_BUILD)

#define REG_COLOUR_VALUES(c, C, val_f, val_b)                                  \
  {L"" L##c L"#", (val_b)}, {L"" L##C L"#", (val_b | CPF_bgi)},                \
      {L"" L##c, (val_f)}, {L"" L##c L"r", (val_f | CPF_Rb)},                  \
      {L"" L##c L"b", (val_f | CPF_Bb)}, {L"" L##c L"g", (val_f | CPF_Gb)},    \
      {L"" L##c L"y", (val_f | CPF_Yb)}, {L"" L##c L"m", (val_f | CPF_Mb)},    \
      {L"" L##c L"c", (val_f | CPF_Cb)}, {L"" L##c L"w", (val_f | CPF_Wb)},    \
      {L"" L##C, (val_f | CPF_fgi)},                                           \
      {L"" L##C L"r", ((val_f | CPF_fgi) | CPF_Rb)},                           \
      {L"" L##C L"b", ((val_f | CPF_fgi) | CPF_Bb)},                           \
      {L"" L##C L"g", ((val_f | CPF_fgi) | CPF_Gb)},                           \
      {L"" L##C L"y", ((val_f | CPF_fgi) | CPF_Yb)},                           \
      {L"" L##C L"m", ((val_f | CPF_fgi) | CPF_Mb)},                           \
      {L"" L##C L"c", ((val_f | CPF_fgi) | CPF_Cb)},                           \
      {L"" L##C L"w", ((val_f | CPF_fgi) | CPF_Wb)},                           \
      {L"" L##c L"R", (val_f | CPF_Rbi)}, {L"" L##c L"B", (val_f | CPF_Bbi)},  \
      {L"" L##c L"G", (val_f | CPF_Gbi)}, {L"" L##c L"Y", (val_f | CPF_Ybi)},  \
      {L"" L##c L"M", (val_f | CPF_Mbi)}, {L"" L##c L"C", (val_f | CPF_Cbi)},  \
      {L"" L##c L"W", (val_f | CPF_Wbi)},                                      \
      {L"" L##C L"R", ((val_f | CPF_fgi) | CPF_Rbi)},                          \
      {L"" L##C L"B", ((val_f | CPF_fgi) | CPF_Bbi)},                          \
      {L"" L##C L"G", ((val_f | CPF_fgi) | CPF_Gbi)},                          \
      {L"" L##C L"Y", ((val_f | CPF_fgi) | CPF_Ybi)},                          \
      {L"" L##C L"M", ((val_f | CPF_fgi) | CPF_Mbi)},                          \
      {L"" L##C L"M", ((val_f | CPF_fgi) | CPF_Mbi)},                          \
      {L"" L##C L"C", ((val_f | CPF_fgi) | CPF_Cbi)}, {                        \
    L"" L##C L"W", ((val_f | CPF_fgi) | CPF_Wbi)                               \
  }

  const std::map<const _cprintf_::unicode_string_t,
                 _cprintf_::system_color_repr_t>
      _cprintf_::std_token_vals{REG_COLOUR_VALUES("r", "R", CPF_Rf, CPF_Rb),
                                REG_COLOUR_VALUES("g", "G", CPF_Gf, CPF_Gb),
                                REG_COLOUR_VALUES("b", "B", CPF_Bf, CPF_Bb),
                                REG_COLOUR_VALUES("y", "Y", CPF_Yf, CPF_Yb),
                                REG_COLOUR_VALUES("m", "M", CPF_Mf, CPF_Mb),
                                REG_COLOUR_VALUES("c", "C", CPF_Cf, CPF_Cb),
                                REG_COLOUR_VALUES("w", "W", CPF_Wf, CPF_Wb)};

#else // defined(CPF_WINDOWS_BUILD)
// http://www.linuxhomenetworking.com/forums/showthread.php/1095-Linux-console-Colors-And-Other-Trick-s
// http://stackoverflow.com/questions/3506504/c-code-changes-terminal-text-color-how-to-restore-defaults-linux
// http://linuxgazette.net/issue65/padala.html
// http://misc.flogisoft.com/bash/tip_colors_and_formatting
// http://man7.org/linux/man-pages/man4/console_codes.4.html
// http://www.linuxquestions.org/questions/programming-9/get-cursor-position-in-c-947833/
// http://ascii-table.com/ansi-escape-sequences-vt-100.php
// http://invisible-island.net/xterm/ctlseqs/ctlseqs.html
// http://ispltd.org/mini_howto:ansi_terminal_codes

/*<ESC>[{attr};{fg};{bg}m*/
#define REG_COLOUR_VALUES(c, C, i)                                             \
  {L"" #c, L"\x1B[0;0;3" #i "m"}, {L"" #C "r", L"\x1B[0;9" #i ";41m"},         \
      {L"" #c "#", L"\x1B[0;0;4" #i "m"},                                      \
      {L"" #C "#", L"\x1B[0;30;10" #i "m"},                                    \
      {L"" #c "r", L"\x1B[0;3" #i ";41m"},                                     \
      {L"" #c "g", L"\x1B[0;3" #i ";42m"},                                     \
      {L"" #c "b", L"\x1B[0;3" #i ";44m"},                                     \
      {L"" #c "y", L"\x1B[0;3" #i ";43m"},                                     \
      {L"" #c "m", L"\x1B[0;3" #i ";45m"},                                     \
      {L"" #c "c", L"\x1B[0;3" #i ";46m"},                                     \
      {L"" #c "w", L"\x1B[0;3" #i ";47m"}, {L"" #C, L"\x1B[0;0;9" #i "m"},     \
      {L"" #C "g", L"\x1B[0;9" #i ";42m"},                                     \
      {L"" #C "b", L"\x1B[0;9" #i ";44m"},                                     \
      {L"" #C "y", L"\x1B[0;9" #i ";43m"},                                     \
      {L"" #C "m", L"\x1B[0;9" #i ";45m"},                                     \
      {L"" #C "c", L"\x1B[0;9" #i ";46m"},                                     \
      {L"" #C "w", L"\x1B[0;9" #i ";47m"},                                     \
      {L"" #c "R", L"\x1B[0;3" #i ";101m"},                                    \
      {L"" #c "G", L"\x1B[0;3" #i ";102m"},                                    \
      {L"" #c "B", L"\x1B[0;3" #i ";104m"},                                    \
      {L"" #c "Y", L"\x1B[0;3" #i ";103m"},                                    \
      {L"" #c "M", L"\x1B[0;3" #i ";105m"},                                    \
      {L"" #c "C", L"\x1B[0;3" #i ";106m"},                                    \
      {L"" #c "W", L"\x1B[0;3" #i ";107m"},                                    \
      {L"" #C "R", L"\x1B[0;9" #i ";101m"},                                    \
      {L"" #C "G", L"\x1B[0;9" #i ";102m"},                                    \
      {L"" #C "B", L"\x1B[0;9" #i ";104m"},                                    \
      {L"" #C "Y", L"\x1B[0;9" #i ";103m"},                                    \
      {L"" #C "M", L"\x1B[0;9" #i ";105m"},                                    \
      {L"" #C "C", L"\x1B[0;9" #i ";106m"}, {                                  \
    L"" #C "W", L"\x1B[0;9" #i ";107m"                                         \
  }

  extern const std::map<const cpf::type::str_t, cpf::type::colour>
      cpf::intern::std_token_vals{

          /*attributes specifiers*/
          {L"bld", L"\x1B[1m"},       {L"dim", L"\x1B[2m"},
          {L"uln", L"\x1B[4m"},       {L"blk", L"\x1B[5m"},
          {L"rvs", L"\x1B[7m"},       {L"hid", L"\x1B[8m"},
          {L"?bld", L"\x1B[21m"},     {L"?dim", L"\x1B[22m"},
          {L"?uln", L"\x1B[24m"},     {L"?blk", L"\x1B[25m"},
          {L"?rvs", L"\x1B[27m"},     {L"?hid", L"\x1B[28m"},
          REG_COLOUR_VALUES(r, R, 1), REG_COLOUR_VALUES(g, G, 2),
          REG_COLOUR_VALUES(b, B, 4), REG_COLOUR_VALUES(y, Y, 3),
          REG_COLOUR_VALUES(m, M, 5), REG_COLOUR_VALUES(c, C, 6),
          REG_COLOUR_VALUES(w, W, 7)};
#endif // defined(CPF_WINDOWS_BUILD)

  /*
          these apply in situations where a user may wish to write a string
          of characters where the first is one which may mistakably be
          interpreted as part of the attribute specified...
          i.e cprintf("$rred")
          in the above case does one wish to apply red to foreground or red to
     both foregound and background? to resolve this issue the following is used:
          cprintf("$r`red");
          */

  const std::initializer_list<_cprintf_::unicode_string_t>
      _cprintf_::attr_esc_seqs = {
          L"`$", L"`r", L"`g", L"`b", L"`y", L"`m", L"`c",
          L"`w", L"`R", L"`G", L"`B", L"`Y", L"`M", L"`C",
          L"`W", L"`.", L"``", L"`?", L"`f", L"`#", L"`l", /*...$bld -> $b`ld*/
      };

  const std::initializer_list<wchar_t> _cprintf_::std_fmt_specs = {
      'c', 'd', 'e', 'E', 'f', 'F', 'g', 'G', 'i', 'o',
      's', 'u', 'x', 'X', 'a', 'A', 'p', 'n', 'b', 'S'};

  // These are characters that terminate a format specifier: Characters
  // typically found at the end of a more complex FS %#x %.6i %05.2f
  const std::initializer_list<wchar_t> _cprintf_::ext_fmtspec_terms = {
      'd', 'f', 's', 'e', 'o', 'x', 'X', 'i', 'u', 'S'};

  // These are characters that typically contained within i.e. in the middle of
  // more complex format specifiers e.g. %.6i, %05.2f
  const std::initializer_list<wchar_t> _cprintf_::inter_fmt_specs = {
      '+', '-', '.', '*', '#', 'l'};

  const std::initializer_list<wchar_t> _cprintf_::escape_characters = {
      '\a', '\b', '\f', '\n', '\r', '\t', '\v', '\\', '\"', '\0'};

  auto col_ids = {'r', 'g', 'b', 'y', 'm', 'c', 'w',
                  'R', 'G', 'B', 'Y', 'M', 'C', 'W'};

  /*characters that may precede colour identifiers (r, g, b...)
          and '#' in the case of '*'..*/
  auto col_id_prefs = {'.'};

  /*specialised character identifiers*/
  auto schar_ids = {
      'f', 'b', '&', '#', '.', '?',
  };

  /*
          parse-predicate storage type
          */
  typedef std::map<wchar_t,
                   std::function<bool(unicode_string_t const &, int const &)>>
      ppred_t;

  bool pred_isdigit(unicode_string_t const &s, int const &p) {
    return isdigit(s[p]) ? true : false;
  }

  bool pred_colour(unicode_string_t const &s, int const &p) {
    /*a colour char may only be preceded by another or any char contained in
     * col_id_prefs...*/
    return (std::find(col_ids.begin(), col_ids.end(), s[p]) != col_ids.end()) ||
           (std::find(col_id_prefs.begin(), col_id_prefs.end(), s[p]) !=
            col_id_prefs.end());
  }

  /*
          map of predicates for conditional syntax checking based on encountered
          characters
          on parsing.

          the returned value from all predicates signifies whether the
     encountered syntax constitues valid (true) token specification with the
     format string or not (false). Where a return value of false would, in
     [most] but not all cases, result in parsing at a particular position in the
     format string being recognised as finished. By "position" it is meant the
     position at which a cprintf syntax token ($) is encountered, incremented by
     1 i.e $r, parsing start position in this case would be at format-string
     index position of character 'r'.

          note: the argument p accepted by all predicates signifies the
     positional index preceding the character at the current search position.
     This "character" is always the [key]
          */
  const ppred_t parsing_predicates = {
      {'f', pred_isdigit},
      {'&', pred_isdigit},
      {'r', pred_colour},
      {'g', pred_colour},
      /*dim blue shall use a different predicate, see below*/
      {'y', pred_colour},
      {'m', pred_colour},
      {'c', pred_colour},
      {'w', pred_colour},
      {'R', pred_colour},
      {'G', pred_colour},
      {'B', pred_colour},
      {'Y', pred_colour},
      {'M', pred_colour},
      {'C', pred_colour},
      {'W', pred_colour},
      {'?',
       [](unicode_string_t const &s, int const &p) -> bool {
         return s[p] == '?' || s[p] == '.';
       }},
      {'.',
       [](unicode_string_t const &s, int const &p) -> bool {
         // s[p] in r, g, b, ... or is digit or b f & etc
         return (std::find(col_ids.begin(), col_ids.end(), s[p]) !=
                 col_ids.end()) ||
                isdigit(s[p]) ||
                (std::find(schar_ids.begin(), schar_ids.end(), s[p]) !=
                 schar_ids.end());
       }},
      {'#',
       [](unicode_string_t const &s, int const &p) -> bool {
         return (std::find(col_ids.begin(), col_ids.end(), s[p]) !=
                 col_ids.end()) ||
                s[p] == '*';
       }},
      {'b', [](unicode_string_t const &s, int const &p) -> bool {
         /*note: this covers the colour blue (...$b[*]...) as well as a token
             representing
             a unix bitmap-colour token (...$34b...).
             in effect, what this means is that the blue colour token shall not
            be treated in a similar manner as the rest of the colours for
            reasons of possible ambiguity in parsing...*/
         return (isdigit(s[p]) ? true : false) ||
                std::find(col_ids.begin(), col_ids.end(), s[p]) !=
                    col_ids.end() ||
                (std::find(col_id_prefs.begin(), col_id_prefs.end(), s[p]) !=
                 col_id_prefs.end());
       }}};

  void purge_meta_esc_sequences(format_string_layout_t & meta) {
    auto purge_str_esc_sequences = [&](unicode_string_t &src) {
      for (auto &es : attr_esc_seqs) {
        auto replacew = es.substr(1);

        /* searches "src" for "es" and replaces it with "replacew" */
        size_t pos = 0;
        while ((pos = search_for(es, src, pos)) != unicode_string_t::npos) {
          src.replace(pos, es.length(), replacew);
          pos += replacew.length();
        }
      }
    };

    for (auto &e : meta) {
      purge_str_esc_sequences(e.second.second);
    }
  }

  /*
          parses "src_string" starting from "ssp" (search start position) and
     returns an offset via "offset_pos" denoting the size of the substring that
     represents an attribute specifier.
          */
  void parse_attribute_specifier(unicode_string_t const &src_string,
                                 unicode_string_t::size_type &offset_val,
                                 unicode_string_t::size_type &ssp) {
    int offset_counter = 0;
    wchar_t c = src_string[ssp]; // first character after occurrance of "$"
    bool finished = false,
         checked_if_is_txt_frmt_modifier = false; //$bld $rvs etc..
    ppred_t::const_iterator
        pred_iter; // colour and screen wipe parsing predicates iterator
    while (!finished) {
      /*we start with checking for text format specifiers first...
                      note: this is only necessary on linux as thats the only
         platform supporting those txt attribute specifiers i.e $rvs $bld,
         $?bld*/
      auto off3 = (ssp + 3);
      auto off4 = (ssp + 4);
      auto lst_i = (src_string.size() - 1);
      // if offset does not exceed searched string's last character index...
      if ((off4 < lst_i || off3 < lst_i) && !checked_if_is_txt_frmt_modifier)

      {
        auto f = [&](unicode_string_t const &s) {
          // faster by using backwards iteration (see initialisation of
          // std_tokens)
          if (std::find(std_tokens.rbegin(), std_tokens.rend(), s) !=
              std_tokens.rend()) {
            finished = true;
            return true;
          }
          return false;
        };

        /*order of if conditions is very important here*/
        if (f(src_string.substr(ssp, 4u)) == true) {
          offset_counter = 4;
        } else if (f(src_string.substr(ssp, 3u)) == true) {
          offset_counter = 3;
        }

        checked_if_is_txt_frmt_modifier = true;
      } else if ((pred_iter = parsing_predicates.find(c)) !=
                 parsing_predicates.end()) {
        do {
          auto off = ssp + (++offset_counter);

          /*on first iteration, take away only 1 from "off" since
                                  character compared to in predicate will not
             have any other preceding char but "$". All other comparisons
             involve comparing the 'current' char with the one preceding its
             position to verify syntax correction
                                  */
          auto x = (offset_counter) == 1 ? 1 : 2;
          /*if character found does abide by parsing rules...*/
          if (pred_iter->second(src_string, off - x) == false) {
            finished = true;
            break;
          }

          c = src_string[off];

        } while ((pred_iter = parsing_predicates.find(c)) !=
                 parsing_predicates.end());
      } else if (isdigit(c)) {
        do {
          auto off = ssp + (++offset_counter);
          c = src_string[off];
        } while (isdigit(c));
      } else {
        finished = true;
      }

      /*
                      if possibly enter an infinite loop...
                      note that we simply use an arbitrary value
                      */
      if (offset_counter >= 50) {
        finished = true;
        offset_counter = 0;
      }
    }

    offset_val = offset_counter;
  }

  /**/
  unicode_string_t parse_fstr_for_attrib_specs(
      unicode_string_t const &format_str_,
      unicode_string_t::size_type search_start_pos_,
      unicode_string_t::size_type &attrib_end_pos) {
    auto ssp = search_start_pos_;

    /*offset from search start position i.e token occurance position in format
     * string*/
    unicode_string_t::size_type offset_pos = 0;

    parse_attribute_specifier(format_str_, offset_pos, ssp);

    /*the parsed attribute(s) string...*/
    unicode_string_t attribute_string = format_str_.substr(ssp, offset_pos);

    if (attribute_string.size() == 0)
      throw CPF_FORMAT_STRING_TOKEN_ERROR; // invalid '$' token encountered on
                                           // parse

    attrib_end_pos += offset_pos;
    return attribute_string;
  }

  format_string_layout_t parse_format_string(
      const unicode_string_t &src_format) {
    format_string_layout_t meta;

    const int NUM_C_TAGS = [&]() -> decltype(NUM_C_TAGS) {
      int occurrences = 0;
      unicode_string_t::size_type start = 0;

      while ((start = search_for(L"$", src_format, start)) !=
             unicode_string_t::npos) {
        ++occurrences;
        start++;
      }
      return occurrences;
    }();

    int token_occurance_pos = 0;
    int attrib_endpos_p1 = 0;
    bool first_iter = true;
    while ((token_occurance_pos = search_for(
                L"$", src_format, attrib_endpos_p1)) != src_format.npos) {
      if (first_iter && token_occurance_pos != 0) {
        unicode_string_vector_t default_attrib{L"?"};
        meta.insert(std::make_pair(
            0u, std::make_pair(default_attrib,
                               src_format.substr(0u, token_occurance_pos))));
        first_iter = false;
      }

      auto tokOccPos_1 = token_occurance_pos + 1;
      unicode_string_t::size_type off = 0;
      auto attibs_str =
          parse_fstr_for_attrib_specs(src_format, token_occurance_pos + 1, off);
      attrib_endpos_p1 = tokOccPos_1 + off;

      auto next_prefix_pos = search_for(L"$", src_format, attrib_endpos_p1);

      /*vector to hold attributes that are applied to the (sub) string
         proceeding the token "$"'s occurance position.*/
      unicode_string_vector_t subseq_str_attribs;
      unicode_string_t current_attrib;

      for (auto c = std::begin(attibs_str); c != std::end(attibs_str); ++c) {
        if (*c == '.') {
          subseq_str_attribs.push_back(current_attrib);
          current_attrib.clear();
          continue;
        }
        current_attrib.append({*c});
        // reached last character in string...
        if (std::distance(attibs_str.begin(), c) == (attibs_str.size() - 1)) {
          subseq_str_attribs.push_back(current_attrib);
        }
      }

      /*the sub string to which the encountered attributes shall be applied*/
      auto meta_substr = src_format.substr(
          attrib_endpos_p1, (next_prefix_pos - attrib_endpos_p1));

      meta.insert({token_occurance_pos, {subseq_str_attribs, meta_substr}});
    }

    if (meta.empty()) {
      meta.insert(std::make_pair(
          0u, std::make_pair(unicode_string_vector_t({L"?"}), src_format)));
    }

    purge_meta_esc_sequences(meta);

    return meta;
  }

  int search_for(const unicode_string_t &_what, const unicode_string_t &_where,
                 const int _offset, const char &_esc_char) {
    bool found = false;
    auto _Off = _offset;
    int position = 0;

    while ((position = _where.find(_what, _Off)) != _where.npos) {
      if (position == 0) {
        return position;
      } else // position > 0
      {
        /*verify that position - 1 is not equal to an escape character*/
        if (_where[position - 1] == _esc_char) {
          _Off = position + 1;
          continue;
        } else {
          return position;
        }
      }
    }

    return position;
  }

  attribute_group_t crnt_txt_attribs;
  static bool glob_terminal_state_restored = true;

#ifdef _WIN32
  static system_color_repr_t saved_terminal_colour;
#endif

  void save_stream_state(file_stream_t user_stream) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    auto s = user_stream == stdout ? stdout_handle : stderr_handle;
    auto ret_okay = GetConsoleScreenBufferInfo(s, &cbsi);

    if (!ret_okay) {
      throw CPF_SYSTEM_ERR; // system call failed to retrieve terminal
                            // attributes
    }
    saved_terminal_colour = cbsi.wAttributes;
#else

#endif
    glob_terminal_state_restored = false;
  }

  void restore_stream_state(file_stream_t user_stream, bool finished_cpf_exec) {
    if (!glob_terminal_state_restored) {
#ifdef _WIN32
      auto s = user_stream == stdout ? stdout_handle : stderr_handle;
      auto ret_okay = SetConsoleTextAttribute(s, saved_terminal_colour);
      if (!ret_okay) {
        throw CPF_SYSTEM_ERR; // failed to restore terminal attributes
      }
#else
      fwprintf(user_stream, L"\x1B[0;0;0m");
#endif
      /*
                      ternary op guarrantees that console settings will be reset
         to originals i.e as they were prior to calling a cprintf function
                      */
      glob_terminal_state_restored = finished_cpf_exec ? true : false;
    }

    /*
                make note of this!
                */
    if (glob_terminal_state_restored)
      std::fflush(user_stream);
  }

  // i.e 32f or 200b
  bool is_bitmap_colour_token(const unicode_string_t &attrib) {
    for (auto c = std::begin(attrib); c != std::end(attrib); ++c) {
      // if any value in the attribute is a digit
      if (isdigit(*c)) {
        return true;
      }
    }
    return false;
  }

  system_color_repr_t get_token_value(const unicode_string_t &colour_key) {
    auto terminal_value = std_token_vals.find(colour_key);
    if (terminal_value == std_token_vals.end()) {
      throw CPF_FORMAT_STRING_TOKEN_ERROR; // invalid token
    }
    return terminal_value->second;
  }

#ifdef CPF_LINUX_BUILD
  unicode_string_t get_terminal_bitmap_colour_value(
      const unicode_string_t &attrib_token) {
    auto at_size = attrib_token.size();
    wchar_t lst_char = attrib_token[at_size - 1];
    auto colour_num = attrib_token.substr(0, at_size - 2);

    auto int_repr = wcstol(colour_num.c_str(), nullptr, 0);

    if ((lst_char != 'f' && lst_char != 'b' && lst_char != '&') ||
        at_size == 1 || (int_repr > 256 || int_repr < 0))
      throw CPF_FORMAT_STRING_TOKEN_ERROR; // invalid attribute token

    unicode_string_t colour_str;

    if (lst_char == 'f') // foreground
    {
      colour_str = (L"\x1B[38;5;" + colour_num + L"m");
    } else if (lst_char == 'b') // background
    {
      colour_str = (L"\x1B[48;5;" + colour_num + L"m");
    } else // "43&"
    {
      colour_str =
          (L"\x1B[38;5;" + colour_num + L"m\x1B[48;5;" + colour_num + L"m");
    }

    return colour_str;
  }
#endif

  void config_text_attribute(file_stream_t user_stream,
                             const system_color_repr_t &user_colour,
                             int col_config_type = 255) {
#ifdef _WIN32

    // interesting:
    // http://comp.os.ms-windows.programmer.win32.narkive.com/1bOxy0qZ/extended-attributes-all-broken-console-api
    auto output_stream_handle =
        user_stream == stdout ? stdout_handle : stderr_handle;

    CONSOLE_SCREEN_BUFFER_INFOEX cbie;
    cbie.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    GetConsoleScreenBufferInfoEx(output_stream_handle, &cbie); // get info

    switch (col_config_type) {
    case CONFIG_FG: {
      // we must first cancel out all foreground text attributes then set
      // foreground attribute to user-specified colour.
      cbie.wAttributes &= ~(FOREGROUND_RED | FOREGROUND_GREEN |
                            FOREGROUND_BLUE | FOREGROUND_INTENSITY);
      cbie.wAttributes |= (user_colour);

      SetConsoleTextAttribute(output_stream_handle, cbie.wAttributes);
    } break;
    case CONFIG_BG: {
      cbie.wAttributes &= ~(BACKGROUND_RED | BACKGROUND_GREEN |
                            BACKGROUND_BLUE | BACKGROUND_INTENSITY);
      cbie.wAttributes |= (user_colour);
      SetConsoleTextAttribute(output_stream_handle, cbie.wAttributes);
    } break;
    case CONFIG_FGBG: {
      cbie.wAttributes = user_colour;
      SetConsoleTextAttribute(output_stream_handle, user_colour);
    } break;
    }
#else

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
    /*colour is controled via Control Sequences*/
    fwprintf(user_stream, user_colour.c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif

#endif
  }

  CPF_API void configure(file_stream_t user_stream,
                         const attribute_group_t &attribute_group) {

    if (crnt_txt_attribs != attribute_group) {
      crnt_txt_attribs = attribute_group;

      for (auto a = std::begin(attribute_group); a != std::end(attribute_group);
           ++a) {
        auto tok = *a;

        if (tok == L"?") /*restore colour to system default*/
        {
          restore_stream_state(user_stream);
        } else /*set colour attribute(s)*/
        {
          auto is_bmct = is_bitmap_colour_token(tok);
#ifdef _WIN32
          if (is_bmct) {
            // because windows does not support that.
            throw CPF_FORMAT_STRING_TOKEN_ERROR; // invalid token
          }

          int config_type;

          /*configuration type is needed to determine which bitwise
                                  operations to do on setting colour values in
             function config_text_attribute*/
          if (tok.size() == 1) {
            config_type = CONFIG_FG;
          } else if (tok.size() == 2 && tok[tok.size() - 1] != '#') {
            config_type = CONFIG_FGBG;
          } else if (tok[tok.size() - 1] == '#') {
            config_type = CONFIG_BG;
          } else {
            throw CPF_FORMAT_STRING_TOKEN_ERROR; // invalid token
          }

          const system_color_repr_t colour_value = get_token_value(tok);
          config_text_attribute(user_stream, colour_value, config_type);

#else
          unicode_string_t control_sequence;
          if (is_bmct) {
            control_sequence = get_terminal_bitmap_colour_value(tok);
          } else {
            control_sequence = get_token_value(tok);
          }

          config_text_attribute(user_stream, control_sequence);
#endif
        } /*is_cursor_pos_attrib*/
      }
    }
  }

#if CPF_DBG_CONFIG

  CPF_API const unicode_string_t debugging_log_format_string =
#ifndef CPF_WINDOWS_BUILD
      LR"debug_str($cdbg
$g@file:$c	$g*%S$c
$g@time:$c	$g*%S$c-$g*%S$c 
$g@func:$c	$g*%S$c
$g@line:$c	$g*%d$c

>> log: $?)debug_str";
#else  // then use small 's' for string args on windows
      LR"debug_str($cdbg
$g@file:$c	$g*%s$c
$g@time:$c	$g*%s$c-$g*%s$c 
$g@func:$c	$g*%s$c
$g@line:$c	$g*%d$c

>> log: $?)debug_str";
#endif // !CPF_LINUX_BUILD

#endif // CPF_DBG_CONFIG

  // cprintf("Characters:\t%c %%\n", 65);
  int get_num_format_specifiers_in_string(const unicode_string_t &obj) {
    int n = 0;
    int pos = 0;
    while ((pos = search_for(L"%", obj, pos, '%')) < (int)obj.size() &&
           pos >= 0) {
      if (pos == obj.size() - 1) {
        /*this would imply the following: cprintf("foo bar %");*/
        throw CPF_FORMAT_SPECIFIER_ERROR; // invalid format specifier('%')
                                          // position.
      }
      int n_ = n;

      /*entering the while loop implies that a '%' was found successfully
                      which means we check whether the proceeding character is a
         '%' if its not, then we are not attempting to escape the % character
                      using standard printf i.e %%*/
      if (obj[(pos + 1)] != '%') {
        /*implies a case when you have %%%_ where "_" is a format specifier such
         * as d -> %%%d*/
        int p_2 = (pos - 2);
        if (p_2 >= 0) {
          if (obj[(pos - 2)] == '%') {
            n_ = ++n;
          }
        }

        if (n_ == n)
          ++n;
      }
      ++pos;
    }
    return n;
  }

  unicode_string_t write_substring_before_format_specifier(
      file_stream_t file_stream, unicode_string_t & printed_string_, int &ssp_,
      const attribute_group_t attr) {
    configure(file_stream, attr);

    ssp_ = search_for(L"%", printed_string_, ssp_, '%');
    if (ssp_ != 0) {
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
      fwprintf(file_stream, printed_string_.substr(0, ssp_).c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
    }

    auto is_in = [=](const wchar_t &_target,
                     const std::initializer_list<wchar_t> &_list) -> bool {
      for (auto &i : _list) {
        if (i == _target) {
          return true;
        }
      }
      return false;
    };

    /* format specifiers beging with % and end with a space*/
    auto space_pos = printed_string_.find(' ', ssp_);
    int offset = 0;
    for (const auto &bfs : std_fmt_specs) {
      if (printed_string_[ssp_ + 1] == bfs) {
        offset = 2;
        break;
      }
    }

    if (offset == 0) {
      unicode_string_t specifier;
      bool parsed_complete_f_spec = false;
      for (const auto &xfst : ext_fmtspec_terms) {
        auto ps = printed_string_.substr(ssp_ + 1);

        auto _max = (ps.size());
        for (auto i(0u); i < _max; ++i) {
          auto crnt_char = ps[i];
          if (isalnum(crnt_char) || is_in(crnt_char, inter_fmt_specs) ||
              crnt_char == xfst) {
            specifier.append({crnt_char});

            if (crnt_char == xfst || is_in(crnt_char, ext_fmtspec_terms)) {
              parsed_complete_f_spec = true;
              break;
            }
          } else {
            throw CPF_FORMAT_SPECIFIER_ERROR; // invalid format specifier detail
          }

          // last iteration
          if (i == (_max - 1) && !is_in(crnt_char, ext_fmtspec_terms)) {
            throw CPF_FORMAT_SPECIFIER_ERROR; // invalid format specifier
          }
        }
        if (parsed_complete_f_spec) {
          break;
        }

        specifier.clear();
      }

      offset = specifier.size() + 1;
    }

    auto fstr = printed_string_.substr(ssp_, offset);
    ssp_ += offset;
    return fstr;
  }

  void write_substring_after_format_specifier(
      file_stream_t file_stream, unicode_string_t & printed_string_, int &ssp_,
      bool &more_args_on_iter,
      format_string_layout_t::const_iterator &format_string_layout_iterator,
      const format_string_layout_t::const_iterator &end_point_comparator) {
    printed_string_ = printed_string_.substr(ssp_);
    ssp_ = 0;

    more_args_on_iter =
        get_num_format_specifiers_in_string(printed_string_) > 0;
    if (!more_args_on_iter) {
      if (!printed_string_.empty()) {
#ifdef CPF_LINUX_BUILD
/*see: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
        fwprintf(file_stream, printed_string_.c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
        printed_string_.clear();
      }
      std::advance(format_string_layout_iterator, 1);
    }
  }

  void write_substring_without_format_specifier(
      file_stream_t file_stream, unicode_string_t & printed_string_, int &ssp_,
      format_string_layout_t::const_iterator &format_string_layout_iterator) {
    configure(file_stream, format_string_layout_iterator->second.first);

    ssp_ = 0;

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
    fwprintf(file_stream, printed_string_.c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
    std::advance(format_string_layout_iterator, 1);

    while (get_num_format_specifiers_in_string(
               format_string_layout_iterator->second.second) == 0) {
      configure(file_stream, format_string_layout_iterator->second.first);
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
      fwprintf(file_stream,
               format_string_layout_iterator->second.second.c_str());
#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
      std::advance(format_string_layout_iterator, 1);
    }
  }

  unicode_string_t ascii_to_unicode_string_conversion(ascii_string_t && mbstr) {
    /*
                The mbstate_t is a trivial non-array type that can represent any
       of the conversion states that can occur in an implementation-defined set
       of supported multibyte character encoding rules. Zero-initialized value
       of mbstate_t represents the initial conversion state, although other
       values of mbstate_t may exist that also represent the initial conversion
       state. Possible implementation of mbstate_t is a struct type holding an
       array representing the incomplete multibyte character, an integer counter
       indicating the number of bytes in the array that have been processed, and
       a representation of the current shift state. type mbstate_t is a trivial
       non-array type that can represent any of the conversion states that can
       occur in an implementation-defined set of supported multibyte character
       encoding rules. Zero-initialized value of mbstate_t represents the
       initial conversion state, although other values of mbstate_t may exist
       that also represent the initial conversion state.
                */
    std::mbstate_t state = std::mbstate_t();
    const char *_mbstr = &mbstr[0];
    // Converts a null-terminated multibyte character sequence, which begins in
    // the conversion state
    int len = 1 + std::mbsrtowcs(NULL, &_mbstr, 0, &state);
    unicode_string_t wstr(len, 0);
    std::mbsrtowcs(&wstr[0], &_mbstr, wstr.size(), &state);
    return wstr;
  }

  CPF_API void format_specifier_correspondence_check(
      unicode_character_string_ptr_t format) {
    for (; *format; ++format) {
      if (*format != '%' || *++format == '%') {
        continue;
      }
      throw CPF_ARG_ERR; // bad format specifier
    }
  }

  unicode_string_t ascii_to_unicode_string_conversion(unicode_string_t && src) {
    return std::move(src);
  }

  // printing strings with lower case 's' as format specifier leads
  // to undefined behaviour when using wide character strings .
  unicode_string_t resolve_string_type_format_specifier(
      const unicode_string_t &fs) {
    unicode_string_t f = fs;
// seems like msvc does not conform to wide character
// format specifier rules
#ifndef CPF_WINDOWS_BUILD
    if (fs.find('s') != unicode_string_t::npos) {
      f.clear();
      for (std::wint_t i(0); i < fs.size(); ++i) {
        f.append(
            {std::iswalpha(fs[i]) ? (wchar_t)std::towupper(fs[i]) : fs[i]});
      }
    }
#endif
    return f;
  }

  template <>
  void write_variadic_argument_to_file_stream<unicode_string_t>(
      file_stream_t file_stream, unicode_string_t const &format,
      unicode_string_t &&arg) {
    write_variadic_argument_to_file_stream(
        file_stream, format, std::forward<const wchar_t *>(arg.c_str()));
  }

  template <>
  void write_variadic_argument_to_file_stream<ascii_string_t>(
      file_stream_t file_stream, unicode_string_t const &format,
      ascii_string_t &&arg) {
    write_variadic_argument_to_file_stream<unicode_string_t>(
        file_stream, format,
        std::forward<unicode_string_t>(ascii_to_unicode_string_conversion(
            std::forward<ascii_string_t>(arg))));
  }

  template <>
  void write_variadic_argument_to_file_stream<char *>(
      file_stream_t file_stream, unicode_string_t const &format, char *&&arg) {
    write_variadic_argument_to_file_stream<unicode_string_t>(
        file_stream, format,
        std::forward<unicode_string_t>(
            ascii_to_unicode_string_conversion(std::forward<char *>(arg))));
  }

  template <>
  void write_variadic_argument_to_file_stream<const char *>(
      file_stream_t file_stream, unicode_string_t const &format,
      const char *&&arg) {
    write_variadic_argument_to_file_stream<unicode_string_t>(
        file_stream, format,
        std::forward<unicode_string_t>(ascii_to_unicode_string_conversion(
            std::forward<const char *>(arg))));
  }

  template <>
  void write_variadic_argument_to_file_stream<signed char *>(
      file_stream_t file_stream, unicode_string_t const &format,
      signed char *&&arg) {
    write_variadic_argument_to_file_stream<unicode_string_t>(
        file_stream, format,
        std::forward<unicode_string_t>(
            ascii_to_unicode_string_conversion(reinterpret_cast<char *>(arg))));
  }

  template <>
  void write_variadic_argument_to_file_stream<const signed char *>(
      file_stream_t file_stream, unicode_string_t const &format,
      const signed char *&&arg) {
    write_variadic_argument_to_file_stream<unicode_string_t>(
        file_stream, format,
        std::forward<unicode_string_t>(ascii_to_unicode_string_conversion(
            reinterpret_cast<const char *>(arg))));
  }

  CPF_API void print_format_string_layout(
      file_stream_t file_stream,
      const format_string_layout_t::const_iterator &end_point_comparator,
      format_string_layout_t::const_iterator &format_string_layout_iterator,
      const unicode_string_t printed_string = L"",
      const int search_start_pos = 0) {
    while (format_string_layout_iterator != end_point_comparator) {
      configure(file_stream, format_string_layout_iterator->second.first);

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

      fwprintf(file_stream,
               format_string_layout_iterator->second.second.c_str());

#ifdef CPF_LINUX_BUILD
#pragma GCC diagnostic pop
#endif
      std::advance(format_string_layout_iterator, 1);
    }

    /*restore defaults*/
    configure(file_stream, unicode_string_vector_t({L"?"}));
  }
} // namespace _cprintf_