#include "c_printf.h"
#include <stdlib.h>     /* atoi */
#include <algorithm>

extern "C" _cpf_types::colour _cpf_sys_attribs = S_T_A_UNDEF;
extern "C" _cpf_types::_string_type_ g_current_colour_repr = S_T_A_UNDEF;

#ifdef _WIN32

auto console_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
auto console_stderr = GetStdHandle(STD_ERROR_HANDLE);

#else /*	#ifdef _WIN32	*/

#endif /*	#ifdef _WIN32	*/

extern "C" void _cpf_authenticate_format_string(const char* format)
{
	for	(; *format; ++format)
	{
		if (*format != '%' || *++format == '%')
		{
			continue;
		}
		throw std::invalid_argument("bad format specifier");
	}
}

_cpf_types::_string_type_ _cpf_do_block_space_parse(
	const _cpf_types::_string_type_ &src_format)
{
	_cpf_types::_string_type_ output("");
	_cpf_types::_string_type_ bs_tag = "/_";
	auto x = 0;
	std::size_t pos = 0;
	bool first_iter = true;
	while ((pos = src_format.find(bs_tag, x)) != src_format.npos)
	{
		if (pos != 0 && first_iter)
		{
			first_iter = false;
			output.append(src_format.substr(0, pos));
		}

		x = src_format.find_first_of(']', pos);
		int i = pos + bs_tag.size();
		_cpf_types::_string_type_ blk_size, s;
		s = src_format.substr(i, x - i);
		bool bright = false;
		int _p = 1;
		_cpf_types::_string_type_ lst{ *(s.end() - _p) };
		_cpf_types::_string_type_ colour = lst;
		colour.resize(2, colour[0]);
		if (lst == "!")
		{
			_p = 2;
			bright = true;
			colour = *(s.end() - _p) ;
			colour.append("!");

			colour += colour;
		}

		blk_size = src_format.substr(
			i,
			std::distance(s.begin(), (s.end() - _p))
			);

		auto rblk_sze = atoi(blk_size.c_str());
		_cpf_types::_string_type_ s_;
		s_.resize(rblk_sze, '-');
		output.append("/" + colour + "]" + s_ + "/!]");

		auto t = src_format.find(bs_tag, x + 1);
		auto t_ = src_format.substr(x + 1, t);
		output.append(t_);
	}

	return output.size() != 0 ? output : src_format;;
}

_cpf_types::meta_format_type _cpf_do_colour_token_parse(
	const _cpf_types::_string_type_ &formatter)
{
	_cpf_types::meta_format_type meta;

	_cpf_types::_string_type_ _c_prefix = "/", _c_suffix = "]";

	const std::size_t NUM_C_TAGS = [&]() -> decltype(NUM_C_TAGS)
	{
		std::size_t occurrences = 0;
		_cpf_types::_string_type_::size_type start = 0;

		while ((start = formatter.find(_c_prefix, start)) != _cpf_types::_string_type_::npos)
		{
			++occurrences;
			start += _c_prefix.length();
		}
		return occurrences;
	}();

	auto first_c_frmt_pos = formatter.find(_c_prefix);
	if (first_c_frmt_pos != 0)
	{
		meta.insert(
			std::make_pair(0, std::make_pair("!", formatter.substr(0, first_c_frmt_pos)))
			);
	}

	std::size_t counter = 0;
	for (auto &c_repr : _cpf_colour_tokens)
	{
		if (counter > NUM_C_TAGS)
		{
			break;
		}
		auto c_frmt = _c_prefix + c_repr + _c_suffix;
		auto pos = formatter.find(c_frmt);

		while (pos != formatter.npos)
		{
			auto p_ = pos;
			auto p_offset = p_ + c_frmt.length();
			pos = formatter.find(_c_prefix, p_offset);
			auto cf = formatter.substr(p_, c_frmt.length());

			if (cf == c_frmt)
			{
				meta.insert(
					std::make_pair(	p_offset, //first
									std::make_pair(	c_repr, //second (first)
													formatter.substr(p_offset, //(second) 
																	(pos - p_offset))
													)
								)
					);
				pos = formatter.find(c_frmt, p_offset);
				++counter;
			}
		}
	}

	return meta;
}

_cpf_types::meta_format_type _cpf_do_cursor_position_parse(
	const _cpf_types::_string_type_ &formatter )
{
	return _cpf_types::meta_format_type();
}

extern "C" std::size_t _cpf_get_num_arg_specifiers(
	const _cpf_types::_string_type_ & obj, 
	const _cpf_types::_string_type_ & target)
{
	std::size_t n = 0;
	_cpf_types::_string_type_::size_type pos = 0;
	while ((pos = obj.find(target, pos)) != _cpf_types::_string_type_::npos)
	{
		n++;
		pos += target.size();
	}
	return n;
}

extern "C" void _cpf_store_attribs(void)
{
	if (_cpf_sys_attribs == S_T_A_UNDEF)
	{
#ifdef _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(console_stdout, &csbi);
		auto a = csbi.wAttributes;
		_cpf_sys_attribs = static_cast<colour_t>(a % 16);
#else
		/*TODO:*/
#endif
	}
}

extern "C" void _cpf_load_attribs(void)
{
#ifdef _WIN32
	for (auto &handle : {console_stdout, console_stderr})
	{
		SetConsoleTextAttribute(handle, _cpf_sys_attribs);
	}
#else
	for (auto s : {stderr, stdout})
	{
		fprintf(s, "\x1B[0m");
	}
	
#endif
}

extern "C" void _cpf_config_terminal(	_cpf_types::stream strm, 
										const _cpf_types::_string_type_ c_repr)
{
	if (g_current_colour_repr.compare(c_repr) != 0)
	{
		g_current_colour_repr = c_repr;
	}

#ifdef _WIN32
	HANDLE h;//TODO: test this for mem leaks
	if (strm == stdout)
	{
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	else if (strm == stderr)
	{
		h = GetStdHandle(STD_ERROR_HANDLE);
	}

	SetConsoleTextAttribute(h, _cpf_colour_token_vals.find(c_repr)->second);
#else
	fprintf(strm, 
			"%s", 
			_cpf_colour_token_vals.find(g_current_colour_repr)->second.c_str());
#endif
}

_cpf_types::error _cpf_call_(	
	_cpf_types::stream strm,
	const _cpf_types::meta_format_type::const_iterator &end_point_comparator,
	_cpf_types::meta_format_type::const_iterator &meta_iter,
	const _cpf_types::_string_type_ printed_string,
	const std::size_t search_start_pos)
{
    int err = 0;
    while(meta_iter != end_point_comparator)
    {
        _cpf_config_terminal(strm, meta_iter->second.first);
        err = fprintf(strm, "%s", meta_iter->second.second.c_str());
        meta_iter++;
    }

	_cpf_load_attribs();

	return 0;
}
