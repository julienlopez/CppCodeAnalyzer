#include "exception.hpp"

Exception::Exception(const std::string& mess): std::exception(), m_message(mess)
{}

Exception::Exception(std::string&& mess): std::exception(), m_message(std::move(mess))
{}

const char* Exception::what() const noexcept
{
	return m_message.c_str();
}

std::string Exception::message() const noexcept
{
	return m_message;
}