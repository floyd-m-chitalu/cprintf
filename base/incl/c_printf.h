#ifndef C_PRINTF_H
#define C_PRINTF_H

#include <tuple>
#include <cstdio>
#include "parse.h"

extern "C" _cpf_types::_string_type_ g_current_colour_repr;
extern "C" const _cpf_types::string_vector _cpf_colour_tokens;

/*

*/
extern "C" void _cpf_store_attribs(void);

/*

*/
extern "C" void _cpf_load_attribs(void);

/*

*/
extern "C" bool _cpf_is_fstream(_cpf_types::stream strm);

/*
	configure system terminal settings
	
	@strm 	- output stream
	@c_repr - colour token string used to locate corresponding value
*/
extern "C" void _cpf_config_terminal(	_cpf_types::stream strm, 
										const _cpf_types::_string_type_ c_repr);

/*

*/
extern "C" std::size_t _cpf_get_num_arg_specifiers(	const _cpf_types::_string_type_ & obj, 
													const _cpf_types::_string_type_ & str);

/*

*/
extern _cpf_types::_string_type_ _cpf_print_pre_arg_str(_cpf_types::stream strm,
														_cpf_types::_string_type_& printed_string_,
														std::size_t& ssp_);

/*

*/
extern void _cpf_print_post_arg_str(_cpf_types::stream strm,
									_cpf_types::_string_type_& printed_string_,
									std::size_t& ssp_,
									bool &more_args_on_iter,
									_cpf_types::meta_format_type::const_iterator &msd_iter);

/*

*/
extern void _cpf_print_non_arg_str(	_cpf_types::stream strm,
									_cpf_types::_string_type_& printed_string_,
									std::size_t& ssp_,
									_cpf_types::meta_format_type::const_iterator &msd_iter);

/*

*/
extern void _cpf_call_(	
	_cpf_types::stream strm,
	const _cpf_types::meta_format_type::const_iterator &end_point_comparator,
	_cpf_types::meta_format_type::const_iterator &msd_iter,
	const _cpf_types::_string_type_ printed_string,
	const std::size_t search_start_pos);

/*

*/
template<typename T0, typename ...Ts>
void _cpf_call_(	
	_cpf_types::stream strm,
	const _cpf_types::meta_format_type::const_iterator &end_point_comparator,
	_cpf_types::meta_format_type::const_iterator &msd_iter,
    const _cpf_types::_string_type_ printed_string,
	const std::size_t search_start_pos,
	T0&& arg0,
    Ts&&... args)
{
	_cpf_config_terminal(strm, msd_iter->second.first);

	_cpf_types::_string_type_ printed_string_ = printed_string;

	/*printed string argument ('%') count*/
	const auto pstr_argc = _cpf_get_num_arg_specifiers(printed_string_, "%");
	
	/*more printf args to print using "printed_string_" as format	*/
	bool more_args_on_iter = false;

	/*boolean used to determine whether variadic arg0 has been passed to printf as an argument yet	*/
	bool printed_arg0 = false;

	/*string parsing start position...*/
    auto ssp_ = search_start_pos;

	if (pstr_argc >= 1)
	{
		auto fstr = _cpf_print_pre_arg_str(strm, printed_string_, ssp_);
		fprintf(strm, fstr.c_str(), arg0);
		_cpf_print_post_arg_str(strm, printed_string_, ssp_, more_args_on_iter, msd_iter);
		printed_arg0 = true;
	}
	else
	{
		_cpf_print_non_arg_str(strm, printed_string_, ssp_, msd_iter);
	}

	bool iter_reached_end = (msd_iter == end_point_comparator);
	auto i_raw_str = iter_reached_end ? "" : msd_iter->second.second;
	if (printed_arg0)
    {
		_cpf_call_(
			strm, end_point_comparator, msd_iter,
			(!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
            more_args_on_iter, std::forward<Ts>(args)...);
    }
	else
    {
		_cpf_call_(
			strm, end_point_comparator, msd_iter,
			(!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
			ssp_, arg0, std::forward<Ts>(args)...);
    }
}

/*

*/
template<typename... Ts>
void c_printf(	_cpf_types::stream strm, std::string format, Ts... args)
{
	if (strm == nullptr)
	{ 
		throw std::invalid_argument("output stream undefined");
	}
	else if(format.empty())
	{
		throw std::invalid_argument("format undefined");
	}

#if defined(_DEBUG)
	assert(
		(_cpf_get_num_arg_specifiers(format, "%") > 0 && sizeof...(args) > 0) ||
		(_cpf_get_num_arg_specifiers(format, "%") == 0 && sizeof...(args) == 0)
		);
	//check_printf(format, normalize_arg(args)...);
#endif

	auto tokenised_string_data = _cpf_colour_token_parse(
		std::forward<_cpf_types::_string_type_>(_cpf_tag_map_token_parse(
			std::forward<_cpf_types::_string_type_>(_cpf_block_space_token_parse(format)))));
    
	auto tsd_iter_begin = tokenised_string_data.cbegin();
	auto tsd_iter_end_point_comparator = tokenised_string_data.cend();

	_cpf_call_(	strm,
				tsd_iter_end_point_comparator,
				tsd_iter_begin,
				tsd_iter_begin->second.second,
				0,
				std::forward<Ts>(normalize_arg(args))...);
}

#ifdef _DEBUG

#include <stdarg.h>

const auto _cpf_debug_pre_str =
R"debug_str(
cpf dbg call 
#build %s-%s
 
@file:		%s
@line-number:	%d
@function:	%s

debug log:

)debug_str";

#ifdef _WIN32
struct _cpf_dbg_fpath_separator
{
	bool operator()(char character) const{ return character == '\\' || character == '/'; }
};

#else

struct _cpf_dbg_fpath_separator
{
	bool operator()(char character) const{	return character == '/';	}
};

#endif

/*
	implicit conversions from std::string to const char* are invalid
	*/
#define debug_c_printf(format, ...) \
	do{\
	std::string const& pathname = __FILE__;\
	auto fname =  std::string(\
	std::find_if(pathname.rbegin(), pathname.rend(),\
	_cpf_dbg_fpath_separator()).base(),\
	pathname.end());\
	fprintf(stderr, _cpf_debug_pre_str, __DATE__, __TIME__, fname.c_str(), __LINE__, __FUNCTION__); \
	}while (0);\
c_printf(stderr, format, ##__VA_ARGS__);

#else

#define debug_c_printf(format, ...) /*do nothing*/

#endif

#endif /* C_PRINTF_H */