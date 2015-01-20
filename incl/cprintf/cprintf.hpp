/*

Copyright (C) 2014 Floyd Mulenga Chitalu jnr									

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

#ifndef __CPF_H__
#define __CPF_H__

#ifndef NDEBUG
#include <cstdarg> //only used by debug functions
#endif

#include <tuple>
#include <memory>
#include <algorithm>
#include <type_traits>


#if !defined(__gnu_linux__)
/*
Note:	GCC does not yet support multi-byte conversion functionality from the following 
		header, as a result narrow-string variants of cprintf's API will do nothing until 
		this is resolved.
*/
#include <codecvt> //wstring_convert
#endif

#include <cprintf/internal/_cpf_parse.h>
#include <cprintf/internal/_cpf_verify.h>
#include <cprintf/internal/_cpf_config.h>

namespace cpf
{
	namespace intern
	{
		/*
			narrow character string debug log
		*/
		CPF_API const cpf::type::nstr dbg_log_fmt_nstr;

		/*
			wide character string debug log
		*/
		CPF_API const cpf::type::str dbg_log_fmt_str;

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
			using type = typename make_seq_indices<Begin + 1, End, I<Indices..., Begin>>::type;
		};

		template <std::size_t Begin,std::size_t End,typename Indices>
		struct make_seq_indices<Begin, 
								End,
								Indices,
								typename std::enable_if<Begin == End, void>::type>
		{
			using type = Indices;
		};

		/*
			to obtain a list of indices.The type alias is defined as :
		*/
		template <std::size_t Begin, std::size_t End>
		using make_seq_indices_T = typename make_seq_indices<Begin, End>::type;

		/*
			It is useful to consider how to pass a set of function arguments to a 
			function or functor.The code to do this is:

			**constexpr
		*/
		template <typename Op, typename... Args>
			inline auto apply(Op&& op, Args&&... args) -> 
		decltype(std::forward<Op>(op)(std::forward<Args>(args)...))
		{
			return std::forward<Op>(op)(std::forward<Args>(args)...);
		}

		/*
			This function overload applies op to all tuple indices...
		*/
		template <	typename Op,
					typename Tuple,
					template <std::size_t...> class I,
					std::size_t... Indices>
		inline auto _apply_tuple(Op&& op, Tuple&& t, I<Indices...>&&) -> 
		decltype(std::forward<Op>(op)(std::get<Indices>(std::forward<Tuple>(t))...))
		{
			return std::forward<Op>(op)(std::get<Indices>(std::forward<Tuple>(t))...);
		}

		/* 
			This function overload forwards op and t along with the
			indices of the tuple generated by make_seq_indices...
		*/
		template <	typename Op,
					typename Tuple,
					typename Indices =	make_seq_indices_T<	0,std::tuple_size<typename std::decay<Tuple>::type>::value>>
		inline auto apply_tuple(Op&& op, Tuple&& t) -> 
		decltype(_apply_tuple(std::forward<Op>(op), std::forward<Tuple>(t), Indices{}))
		{
			return	_apply_tuple(std::forward<Op>(op), std::forward<Tuple>(t), Indices{});
		}

		/*
			convert from narraow character string to wide character string
			@returns wide version of src
		*/
		CPF_API cpf::type::str wconv(const cpf::type::nstr &src);

		/*
			convert from wide character string to narrow character string
			@returns narrow version of src
		*/
		CPF_API cpf::type::nstr nconv(const cpf::type::str &src);

		/*
			@return number of printf argument tokens "%" in a given string
		*/
		CPF_API cpf::type::size get_num_arg_specs(	const cpf::type::str & str);

		/*
			print the substring preceding an argument specifier in a sub-format-string
		*/
		CPF_API cpf::type::str write_pre_arg_str(	cpf::type::stream ustream,
													cpf::type::str& printed_string_,
													cpf::type::size& ssp_,
													const cpf::type::attribute_group attr);

		/*
			print the substring proceding an argument specifier in a sub-format-string 
		*/
		CPF_API void write_post_arg_str(cpf::type::stream ustream,
										cpf::type::str& printed_string_,
										cpf::type::size& ssp_,
										bool &more_args_on_iter,
										cpf::type::meta::const_iterator &meta_iter,
										const cpf::type::meta::const_iterator &end_point_comparator);

		/*
			print non-argument specifying format string i.e where the implmentation
			need not invoke printf with any avariadic arguments.
		*/
		CPF_API void write_non_arg_str(	cpf::type::stream ustream,
										cpf::type::str& printed_string_,
										cpf::type::size& ssp_,
										cpf::type::meta::const_iterator &meta_iter);

		template<typename T>
		void write_arg(	cpf::type::stream ustream, 
						cpf::type::str const &format, 
						T&& arg)
		{
			std::fwprintf(ustream, format.c_str(), arg);
		}

		/*
			some tiny extra wizardry has to be done before printing the following types...
		*/
		template<>
		void write_arg<cpf::type::str>(	cpf::type::stream ustream,
										cpf::type::str const &format,
										cpf::type::str&& arg);

		template<>
		void write_arg<cpf::type::nstr>(cpf::type::stream ustream,
										cpf::type::str const &format,
										cpf::type::nstr&& arg);

		template<>
		void write_arg<char*>(	cpf::type::stream ustream,
								cpf::type::str const &format,
								char*&& arg);

		template<>
		void write_arg<signed char*>(	cpf::type::stream ustream,
										cpf::type::str const &format,
										signed char*&& arg);

		template<>
		void write_arg<const char*>(cpf::type::stream ustream,
									cpf::type::str const &format,
									const char*&& arg);

		template<>
		void write_arg<const signed char*>(	cpf::type::stream ustream,
											cpf::type::str const &format,
											const signed char*&& arg);

		/*
			recursion-terminating function (counterpart to "update_ustream" with variadic arguments). 
			This is the function executated when the API is called with only a format 
			string and no arguments.
		*/
		CPF_API void update_ustream(cpf::type::stream ustream,
									const cpf::type::c_meta_iterator &end_point_comparator,
									cpf::type::c_meta_iterator &meta_iter,
									const cpf::type::str printed_string,
									const cpf::type::size search_start_pos);

		/*
			recursive call to process the format string as well as every argument provided.
			note: this function is not executed if no variadic arguments are respecified.
			using cfprintf_t and cprintf_t guarrantees the execution of this function.
		*/
		template<typename T0, typename ...Ts>
		void update_ustream(cpf::type::stream ustream,
							const cpf::type::c_meta_iterator &end_point_comparator,
							cpf::type::c_meta_iterator &meta_iter,
							const cpf::type::str printed_string,
							const cpf::type::size search_start_pos,
							T0&& arg0,
							Ts&&... args)
		{
			/*
				----------------------------------
				Compile-Time argument verification
				----------------------------------
			*/
			static_assert(
				/*
					check if argument is a char-type pointer (narrow or wide)
				*/
				(	std::is_pointer<T0>::value and
					(
						std::is_same<wchar_t*, T0>::value			or std::is_same<char*, T0>::value				or
						std::is_same<unsigned char*, T0>::value		or std::is_same<signed char*, T0>::value		or
						std::is_same<const wchar_t*, T0>::value		or std::is_same<const char*, T0>::value			or
						std::is_same<const signed char*, T0>::value	or std::is_same<const unsigned char*, T0>::value
					)
				) or
				/*
					check if argument is of type std::string or std::wstring
				*/
				(
					std::is_same<cpf::type::str, T0>::value or
					std::is_same<cpf::type::nstr, T0>::value
				) or
				/*
					check if argument is of type "float", "double" or "long double"
				*/
				std::is_floating_point<T0>::value or
				/*
					check if argument is of type "char" "short" "int" "long" ("unsigned" included)
				*/
				std::is_integral<T0>::value, 
				/*
					------------------------------
					End Of Type-Check Condition...
					------------------------------
				*/
				"CPF-CT-ERR: Illegal Argument Type!");

			cpf::type::str printed_string_ = printed_string;

			/*	printed string argument-specifier ('%') count	*/
			const auto pstr_argc = cpf::intern::get_num_arg_specs(printed_string_);
	
					/*	
						more variadic args to write using "printed_string_" as 
						format string.
					*/
			bool 	more_args_on_iter = false,
					/*	
						boolean used to signify whether variadic "arg0" has been 
						passed to std::fwprintf as an argument yet	
					*/
					printed_arg0 = false;

			/*	string parsing start position...	*/
			auto ssp_ = search_start_pos;

			if (pstr_argc >= 1)
			{
				auto arg_format_spec = cpf::intern::write_pre_arg_str(	ustream, 
																		printed_string_, 
																		ssp_, 
																		meta_iter->second.first);
				/*
					write the current argument i.e. "arg0" as formatted according to "arg_format_spec"
					to the user stream
				*/
				cpf::intern::write_arg(	ustream,
										arg_format_spec,
										std::forward<T0>(arg0));

				cpf::intern::write_post_arg_str(ustream,
												printed_string_, 
												ssp_, 
												more_args_on_iter,
												meta_iter, 
												end_point_comparator);
				printed_arg0 = true;
			}
			else
			{
				cpf::intern::write_non_arg_str(ustream, printed_string_, ssp_, meta_iter);
			}

			bool iter_reached_end = (meta_iter == end_point_comparator);
			auto i_raw_str = iter_reached_end ? L"" : meta_iter->second.second;

			/*
				note: 	only when "arg0" has been passed to std::fwprintf does 
						variadic-argument based recursion proceed onto the 
						next one subsequently after arg0. Else recurse back
						into this function with the same arguments.
			*/
			if (printed_arg0)
			{
				cpf::intern::update_ustream(ustream, end_point_comparator, meta_iter,
											(!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
											more_args_on_iter, std::forward<Ts>(args)...);
			}
			else
			{
				cpf::intern::update_ustream(ustream, end_point_comparator, meta_iter,
											(!more_args_on_iter && !iter_reached_end) ? i_raw_str : printed_string_,
											ssp_, std::forward<T0>(arg0), std::forward<Ts>(args)...);
			}
		}
	}
}

template<typename... Ts>
void cfwprintf(cpf::type::stream ustream, const cpf::type::str &format, Ts... args)
{
	if (ustream == nullptr) 
		throw cpf::type::except(L"CPF-RT-ERR: output stream is undefined (null)");

	auto meta_format = cpf::intern::process_format_string(format);

#ifndef NDEBUG
	cpf::type::size nargs = 0u;
	for (const auto &i : meta_format)
	{
		nargs += cpf::intern::get_num_arg_specs(i.second.second);
	}

	if (nargs != sizeof...(args))
	{
		throw cpf::type::except(L"CPF-RT-ERR: invalid argument count");
	}

	/*
		format specifier-to-argument correspondence check
		i.e "%d" must correspond to an integral
	*/
	cpf::intern::arg_check(format.c_str(), std::forward<Ts>(args)...);
#endif
	
	auto mf_begin = meta_format.cbegin();
	/*
		end point comparator...
	*/
	auto mf_endpoint_cmp = meta_format.cend();

	/*
		note:	the try catch block is necessary to restore stream
				state should unexpected behaviour occur at runtime. Typical 
				cases are errors in user code.
	*/
	try
	{
		cpf::intern::save_stream_state(ustream);

		/*	
			make actual call to do printing and system terminal configurations	
		*/
		cpf::intern::update_ustream(	ustream,
										mf_endpoint_cmp,
										mf_begin,
										mf_begin->second.second,
										0u,
										std::forward<Ts>(args)...);

		cpf::intern::restore_stream_state(ustream, true);
	}
	catch (cpf::type::except &e)
	{
		/*
			executed only on occurance of a runtime error during function execution.
		*/
		cpf::intern::restore_stream_state(ustream, true);
		throw e;//rethrow!
	}
}

/*
	Writes the string referenced by "format" to the "stream". If "format"
	includes format specifiers (subsequences beginning with %), the
	additional cprintf arguments following "format" are formatted and inserted
	in the resulting string replacing their respective specifiers.
	If format includes attribute specifiers (tokens beginning with $),
	the characters proceding these attibute specifiers will be modified
	in accordance with specification contained in the format.

	After the format parameter, the function expects at least as many
	additional arguments as specified by "format".

	@stream
	Pointer to a FILE object that identifies an output stream.
	@format
	string that contains the text to be written to the stream.
	It can optionally contain embedded format specifiers that
	are replaced by the values specified in subsequent additional
	arguments and formatted as requested or are used as indicators
	of how the output text should appear in a console..

	For further info please refer to:
	http://www.cplusplus.com/reference/cstdio/fprintf/
*/
template<typename... Ts>
inline void cfprintf(cpf::type::stream ustream, const cpf::type::nstr &format, Ts... args)
{
	/*
		GCC does not yet support multi-byte conversion functionality...

		http://stackoverflow.com/questions/24497956/is-codecvt-not-supported-by-clang-or-gcc
		http://stackoverflow.com/questions/15615136/is-codecvt-not-a-std-header?rq=1
		https://gcc.gnu.org/onlinedocs/libstdc++/manual/facets.html#std.localization.facet.codecvt
		https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2011
	*/
#if defined(__gnu_linux__)
	printf("the header \"codecvt\" is missing!\nuse wide char API variants (cwpr...)\n");
	return; //skip
#else
	/*
		*\http://en.cppreference.com/w/cpp/locale/codecvt_utf8
		*\http://stackoverflow.com/questions/402283/stdwstring-vs-stdstring
	*/
	auto converter = std::wstring_convert<std::codecvt_utf8<wchar_t>>();
	auto multibyte_version = converter.from_bytes(format);
	cfwprintf(ustream, multibyte_version.c_str(), args...);
#endif
}

template<typename... Ts>
inline void cwprintf(const cpf::type::str &format, Ts... args)
{
	cfwprintf(stdout, format, std::forward<Ts>(args)...);
}

/*
	Writes the string referenced by "format" to "stdout". If "format"
	includes format specifiers (subsequences beginning with %), the
	additional cprintf arguments following "format" are formatted and inserted
	in the resulting string replacing their respective specifiers.
	If format includes attribute specifiers (tokens beginning with $),
	the characters proceding these attibute specifiers will be modified
	in accordance with specification contained in the format.

	After the format parameter, the function expects at least as many
	additional arguments as specified by "format".

	@format
	string that contains the text to be written to stdout.
	It can optionally contain embedded format and attribute specifiers that are replaced
	by the values specified in subsequent additional arguments and
	formatted as requested or are used as indicators of how the output text
	should appear in a console.
*/
template<typename... Ts>
inline void cprintf(const cpf::type::nstr &format, Ts... args)
{
	cfprintf(stdout, format, std::forward<Ts>(args)...);
}

template<typename... Ts>
inline void cfwprintf_t(cpf::type::stream ustream,
					const cpf::type::str &format,
					cpf::type::arg_pack<Ts...> args_tup)
{
	auto predef_args_tup = std::make_tuple(ustream, format);
	auto call_args = std::tuple_cat(predef_args_tup, args_tup);
	cpf::apply_tuple(cfwprintf<Ts...>, call_args);
}


/*
	Instead of accepting variadic arguments this function expects a tuple
	object. Elements of this object must abide the same restrictions
	imposed on those permitted to cfprintf.

	see cfprintf doc-string for more info.
*/
template<typename... Ts>
inline void cfprintf_t(cpf::type::stream ustream,
				const cpf::type::nstr &format,
				cpf::type::arg_pack<Ts...> args_tup)
{
	auto predef_args_tup = std::make_tuple(ustream, format);
	auto call_args = std::tuple_cat(predef_args_tup, args_tup);
	cpf::intern::apply_tuple(cfprintf<Ts...>, call_args);
}

template<typename... Ts>
inline void cwprintf_t(const cpf::type::str &format, cpf::type::arg_pack<Ts...> args_tup)
{
	cfwprintf_t(stdout, format, std::forward<cpf::type::arg_pack<Ts...>>(args_tup));
}

/*
	Instead of accepting variadic arguments this function expects a tuple
	object. Elements of this object must abide the same restrictions
	imposed on those of cprintf.
	all output is directed to stdout.

	see cprintf doc-string for more info.
*/
template<typename... Ts>
inline void cprintf_t(const cpf::type::nstr &format, cpf::type::arg_pack<Ts...> args_tup)
{		
	cfprintf_t(stdout, format, std::forward<cpf::type::arg_pack<Ts...>>(args_tup));
}

#ifdef __gnu_linux__
/*
	cprintf API variant that provides an extra "safety" feature by requiring that
	users explcitly use only string laterals when invoking the API for the format 
	string.

	By definition, a string literal is any text in double quotes, like "me".
	What is the type of "me"? It is const char[3] (two characters for the 
	letters in the string and one for terminating zero).
*/
template<typename... Ts>
inline void cprintf_s( cpf::type::nstrl format, Ts... args)
{
	cprintf(format, std::forward<Ts>(args)...);
}

#endif

#ifndef	NDEBUG

/*
	The following are auxillary macros ideal for debugging purposes.
	All output is streamed to standard error. Users may use these just as 
	they would cprintf, cfprintf, cprintf_t, and cfprintf_t. Permissions
	and limitations imposed reflect those of the aforementioned.
	Macro expansion shall occur only in client debug builds and non else.
	As such, building in release mode results in the macros	expanding to [nothing], 
	rendering a client call impotent.
*/

#ifdef _WIN32
#define CPF_SEP_COND (character == '\\' || character == '/');
#else
#define CPF_SEP_COND ( character == '/');
#endif

/*
	Note the the anonymous struct for os specific dir path wrangling
*/
#define CPF_DBG_LOG_WRITE(cpf_api_func, log_str_t, log_str)\
	typedef struct {\
	bool operator()(char character) const{\
		return CPF_SEP_COND;\
	}\
	}fpath_sep_func;\
	auto fname =  log_str_t(\
	std::find_if(pathname.rbegin(), pathname.rend(),fpath_sep_func()).base(),\
	pathname.end());\
	cpf_api_func(stderr, log_str, \
	fname.c_str(), __TIME__, __DATE__, __FUNCTION__, __LINE__);

/*
	wide character string variants
*/
#define CPF_DBG_LOG_STR\
	const cpf::type::str pathname = cpf::wconv(__FILE__);\
	CPF_DBG_LOG_WRITE(cfwprintf, cpf::type::str, cpf::intern::dbg_log_fmt_str.c_str())

#define cfwprintf_dbg(ustream, format, ...) \
	do{\
	CPF_DBG_LOG_STR \
	cfwprintf(ustream, format, ##__VA_ARGS__);\
	}while (0);

#define cwprintf_dbg(format, ...) \
	cfwprintf_dbg(stderr, format, ##__VA_ARGS__)

#define cfwprintf_t_dbg(ustream, format, tup) \
	do{\
	CPF_DBG_LOG_STR \
	cfwprintf_t(ustream, format, tup); \
	} while (0);\

#define cwprintf_t_dbg(format, tup) \
	cfwprintf_t_dbg(stderr, format, tup);

/*
	narrow character string variants
*/
#define CPF_DBG_LOG_NSTR\
	const cpf::type::nstr pathname = __FILE__;\
	CPF_DBG_LOG_WRITE(cfprintf, cpf::type::nstr, cpf::intern::dbg_log_fmt_nstr.c_str())

#define cfprintf_dbg(ustream, format, ...) \
	do{\
	CPF_DBG_LOG_NSTR \
	cfprintf(ustream, format, ##__VA_ARGS__);\
	}while (0);

#define cprintf_dbg(format, ...) \
	cfprintf_dbg(stderr, format, ##__VA_ARGS__)

#define cfprintf_t_dbg(ustream, format, tup) \
	do{\
	CPF_DBG_LOG_NSTR \
	cfprintf_t(ustream, format, tup); \
	} while (0);\

#define cprintf_t_dbg(format, tup) \
	cfprintf_t_dbg(stderr, format, tup);

#else // do nothing...

#define cfwprintf_dbg(ustream, format, ...) 
#define cwprintf_dbg(format, ...) 
#define cfwprintf_t_dbg(ustream, format, tup) 
#define cwprintf_t_dbg(format, tup) 

#define cfprintf_dbg(ustream, format, ...) 
#define cprintf_dbg(format, ...) 
#define cfprintf_t_dbg(ustream, format, tup) 
#define cprintf_t_dbg(format, tup) 

#endif /*#ifndef NDEBUG*/

#endif /* __CPF_H__ */
