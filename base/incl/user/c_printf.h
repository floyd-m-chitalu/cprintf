#ifndef C_PRINTF_H
#define C_PRINTF_H

#include <tuple>
#include <algorithm>
#include <stdarg.h>
#include <cassert>
#include "_cpf_parse.h"
#include "_cpf_type_norm.h"

template <std::size_t...>
struct indices
{	 };

template <	std::size_t Begin,
			std::size_t End,
			typename Indices = indices<>,
			typename Enable = void>
struct make_seq_indices
{
	static_assert(Begin <= End, "Begin must be <= End");
};

template <	std::size_t Begin,
			std::size_t End,
			template <std::size_t...> class I,
			std::size_t... Indices>
struct make_seq_indices<Begin, 
						End, 
						I<Indices...>,
						typename std::enable_if<Begin < End, void>::type>
{
	using type =typename make_seq_indices<Begin + 1, End, I<Indices..., Begin>>::type;
};

template <std::size_t Begin,std::size_t End,typename Indices>
struct make_seq_indices<Begin, 
						End,
						Indices,
						typename std::enable_if<Begin == End, void>::type>
{
	using type = Indices;
};

//to obtain a list of indices.The type alias is defined as :
template <std::size_t Begin, std::size_t End>
using make_seq_indices_T = typename make_seq_indices<Begin, End>::type;

//It is useful to consider how to pass a set of function arguments to a function or functor.The code to do this is:
//constexpr
template <typename Op, typename... Args>
inline auto apply(Op&& op, Args&&... args) -> 
decltype(std::forward<Op>(op)(std::forward<Args>(args)...))
{
	return std::forward<Op>(op)(std::forward<Args>(args)...);
}

// This function overload applies op to all tuple indices...
template <
	typename Op,
	typename Tuple,
	template <std::size_t...> class I,
	std::size_t... Indices
>
inline auto _apply_tuple(Op&& op, Tuple&& t, I<Indices...>&&) -> 
decltype(std::forward<Op>(op)(std::get<Indices>(std::forward<Tuple>(t))...))
{
	return std::forward<Op>(op)(std::get<Indices>(std::forward<Tuple>(t))...);
}

// This function overload forwards op and t along with the
// indices of the tuple generated by make_seq_indices...
template <
	typename Op,
	typename Tuple,
	typename Indices =	make_seq_indices_T<	0,std::tuple_size<typename std::decay<Tuple>::type>::value>
>
inline auto apply_tuple(Op&& op, Tuple&& t) -> 
decltype(_apply_tuple(std::forward<Op>(op), std::forward<Tuple>(t), Indices{}))
{
	return	_apply_tuple(std::forward<Op>(op), std::forward<Tuple>(t), Indices{});
}

extern "C" void _cpf_store_sys_default_attribs(_cpf_types::stream strm);

/*

*/
extern "C" std::size_t _cpf_get_num_arg_specifiers(	const _cpf_types::_string_type_ & obj, 
													const _cpf_types::_string_type_ & str);

/*

*/
extern _cpf_types::_string_type_ _cpf_print_pre_arg_str(_cpf_types::stream strm,
														_cpf_types::_string_type_& printed_string_,
														std::size_t& ssp_,
														const _cpf_types::_string_type_ c_repr);

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
	recursive call to process the format string as well as any arguments provided
	note: this function is not executed if no variadic arguments are respecified
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
		auto fstr = _cpf_print_pre_arg_str(strm, printed_string_, ssp_, msd_iter->second.first);
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
	c_printf is a C++(11/0x) language function for extended formatted-printing. 
	It achieves this by providing a thin auxiallary layer atop that of fprintf. 
	The function works,	in much a similar manner to that of its patternal 
	counter-part(s) i.e printf, fprintf etc. 
	Aside from guarranteed type-safety (unlike that of it predecesors) c_printf also
	introduces the feature of colour token specification. 
	With this, users are able to specify, as part of the format string, the colour of
	all or some of the format-string-text within console output. Alongside this is also 
	the addition of	map tokens which enable users the ability to map specific strings with 
	certain token value(s).

	@if colour configuration is enabled, the function will throw a _cpf_err exception 
	on condition that an invalid token is encountered on parsing. In the case that a 
	user opts to use features which are not available for a platform (or terminal), the 
	format string may remain unchanged, the implementation may throw an exception on parsing.
	 
*/
template<typename... Ts>
void c_printf(	_cpf_types::stream strm, const char* format, Ts... args)
{
	assert(strm != nullptr && "output stream undefined");
	assert(format != nullptr && "format string undefined");

#if defined(_DEBUG)
	//this will be used in the first parse stage not here!!
	//_cpf_verify(format, normalize_arg(args)...);
#endif
	auto meta_str_data = _cpf_process_format_string(format);
	auto tsd_iter_begin = meta_str_data.cbegin();
	auto tsd_iter_end_point_comparator = meta_str_data.cend();

	if (_cpf_colour_config == _CPF_ENABLE)
	{
		_cpf_store_sys_default_attribs(strm);
	}
	
	_cpf_call_(	strm,
				tsd_iter_end_point_comparator,
				tsd_iter_begin,
				tsd_iter_begin->second.second,
				0,
				std::forward<Ts>(normalize_arg(args))...);
}

template<typename... Ts>
void c_fprintf(_cpf_types::stream strm, const char* format, Ts... args)
{
	assert(strm != nullptr && "output stream undefined");
	assert(format != nullptr && "format string undefined");

#if defined(_DEBUG)
	//this will be used in the first parse stage not here!!
	//_cpf_verify(format, normalize_arg(args)...);
#endif
	auto meta_str_data = _cpf_process_format_string(format);
	auto tsd_iter_begin = meta_str_data.cbegin();
	auto tsd_iter_end_point_comparator = meta_str_data.cend();

	if (_cpf_colour_config == _CPF_ENABLE)
	{
		_cpf_store_sys_default_attribs(strm);
	}

	_cpf_call_(strm,
		tsd_iter_end_point_comparator,
		tsd_iter_begin,
		tsd_iter_begin->second.second,
		0,
		std::forward<Ts>(normalize_arg(args))...);
}

template<typename... Ts>
void c_printf(const char* format, Ts... args)
{
	c_fprintf(stdout, format, std::forward<Ts>(args)...);
}

template<typename... Ts>
void c_fprintf_t(_cpf_types::stream strm, const char* format, std::tuple<Ts...> args_tup)
{
	auto predef_args_tup = std::make_tuple(strm, format);
	auto call_args = std::tuple_cat(predef_args_tup, args_tup);
	
	apply_tuple(c_fprintf<Ts...>, call_args);
}

template<typename... Ts>
void c_printf_t(const char* format, std::tuple<Ts...> args_tup)
{
	c_fprintf_t(stdout, format, std::forward<std::tuple<Ts...>>(args_tup));
}

#ifdef _DEBUG

const auto _cpf_debug_pre_str =
R"debug_str(
>> dbg print 
@build:		%s-%s 
@file:		%s
@line-number:	%d
@function:	%s

log:
)debug_str";

/*
	os specific dir path wrangling
*/
struct _cpf_dbg_fpath_separator
{
	bool operator()(char character) const
	{ 
#ifdef _WIN32
		return character == '\\' || character == '/'; 
#else
		return character == '/';
#endif
	}
};

/*
	The following are auxillary macros ideal for debugging purposes.
	All output is streamed to standard error by default.
	Users may use this function just as they would c_printf,
	c_fprintf, c_printf_t and c_fprintf_t and permissions
	and limitations imposed reflect those of the aforementioned.
	Macro expansion will only occur in client debug builds and non else.
	And as such, building in release mode results in the macros
	expanding to [nothing], rendering your call impotent.
	*/

#define __print_stat_str\
	std::string const& pathname = __FILE__;\
	auto fname =  std::string(\
	std::find_if(pathname.rbegin(), pathname.rend(),\
	_cpf_dbg_fpath_separator()).base(),\
	pathname.end());\
	fprintf(stderr, _cpf_debug_pre_str, __DATE__, __TIME__, fname.c_str(), __LINE__, __FUNCTION__); \

#define c_fprintf_dbg(strm, format, ...) \
	do{\
	__print_stat_str \
	c_fprintf(strm, format, ##__VA_ARGS__);\
	}while (0);

#define c_printf_dbg(format, ...) \
	c_fprintf_dbg(stderr, format, ##__VA_ARGS__)

#define c_fprintf_t_dbg(strm, format, tup) \
	do{\
	__print_stat_str \
	c_fprintf_t(strm, format, tup); \
	} while (0);\

#define c_printf_t_dbg(format, ...) \
	c_fprintf_t_dbg(stderr, format, ##__VA_ARGS__);

#else

/*do nothing*/
#define c_fprintf_dbg(strm, format, ...) 

#define c_printf_dbg(format, ...) 

#define c_fprintf_t_dbg(strm, format, ...) 

#define c_printf_t_dbg(format, ...) 

#endif

#endif /* C_PRINTF_H */