#include "_cpf_find.h"

bool encountered_esc_seq_on_parse = false;

std::size_t _cpf_find(	const _cpf_type::str& _what, 
						const _cpf_type::str& _where,
						const std::size_t _offset,
						const char& _esc_char)
{
	bool found = false;
	auto _Off = _offset;
	std::size_t position = 0;

	while((position = _where.find(_what, _Off)) != _where.npos)
	{
		if(position == 0)
		{
			return position;
		}
		else //position > 0 
		{
			/*verify that position - 1 is not equal to an escape character*/
			if(_where[position - 1] == _esc_char)
			{
				encountered_esc_seq_on_parse = true;
				_Off = position + 1;
				continue;
			}
			else
			{
				return position;
			}
		}
	}

	return position;
}