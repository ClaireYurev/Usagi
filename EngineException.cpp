#include "UsagiEngineException.h"
#include <sstream>

UsagiException::UsagiException(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

const char* UsagiException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* UsagiException::GetType() const noexcept
{
	return "Usagi Exception";
}

int UsagiException::GetLine() const noexcept
{
	return line;
}

const std::string& UsagiException::GetFile() const noexcept
{
	return file;
}

std::string UsagiException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}