#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

#include <stdexcept>

class Exception : public std::exception
{
public:
	Exception(const std::string& mess);
	Exception(std::string&& mess);

	virtual ~Exception() noexcept = default;

	virtual const char* what() const noexcept override;

	std::string message() const noexcept;

private:
	std::string m_message;
};

#endif