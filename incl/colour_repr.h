#ifndef _CPF_COLOUR_REPR_H
#define _CPF_COLOUR_REPR_H

#include <vector>
#include <map>
#include <string>

namespace _cpf_types
{
	typedef std::string _string_type_;
}

/*S_T_A_UNDEF = system text attribute undefined*/

#ifdef _WIN32

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#undef VC_EXTRALEAN
#undef WIN32_LEAN_AND_MEAN

namespace _cpf_types
{
	typedef WORD colour;
}

#define S_T_A_UNDEF (666)

/*
	auxillary symbols to help shorten code in cpp
	and aid in readability
*/

/*
	*\foreground
*/
#define _cpf_Rf 	(FOREGROUND_RED)
#define _cpf_Gf 	(FOREGROUND_GREEN)
#define _cpf_Bf 	(FOREGROUND_BLUE)
#define _cpf_Yf		(_cpf_Rf | _cpf_Gf)
#define _cpf_Mf		(_cpf_Rf | _cpf_Bf)
#define _cpf_Cf		(_cpf_Gf | _cpf_Bf)
#define _cpf_Wf		(_cpf_Rf | _cpf_Gf | _cpf_Bf)

/*
	*\background
*/
#define _cpf_Rb 	(BACKGROUND_RED)
#define _cpf_Gb 	(BACKGROUND_GREEN)
#define _cpf_Bb 	(BACKGROUND_BLUE)
#define _cpf_Yb		(_cpf_Rb | _cpf_Gb)
#define _cpf_Mb		(_cpf_Rb | _cpf_Bb)
#define _cpf_Cb		(_cpf_Gb | _cpf_Bb)
#define _cpf_Wb		(_cpf_Rb | _cpf_Gb | _cpf_Bb)

/*
	*\intensifiers
*/
#define _cpf_fgi 	(FOREGROUND_INTENSITY)
#define _cpf_bgi	(BACKGROUND_INTENSITY)

#else /*	#ifdef _WIN32	*/

#define S_T_A_UNDEF ("undef")

namespace _cpf_types
{
	typedef std::string colour;
}

#endif /*	#ifdef _WIN32	*/

namespace _cpf_types
{
	typedef std::map<const _cpf_types::_string_type_, _cpf_types::colour> colour_token_map;
	typedef std::vector<_cpf_types::_string_type_> string_vector;
}

extern const _cpf_types::colour_token_map _cpf_colour_token_vals;
extern const _cpf_types::string_vector _cpf_colour_tokens;
extern const _cpf_types::string_vector _cpf_blockspace_tokens;

#endif /*	#ifndef _CPF_COLOUR_REPR_H	*/


/*

#define BLOCK_SPACE_TOKEN "^_6r!|"
//http://en.allexperts.com/q/C-1040/seting-position-cursor-desired.htm
#define CURSOR_POS_TOKEN "^@10-20|"

*/