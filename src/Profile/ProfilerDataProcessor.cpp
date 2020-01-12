#include "Profile/ProfilerDataProcessor.h"

#include <json.hpp>

namespace Profiler
{
	std::vector<std::string> Split(const std::string& a_stringToSplit, char a_splitCharacter);

	ProfilerDataProcessor::ProfilerDataProcessor()
	{
	}

	ProfilerDataProcessor::~ProfilerDataProcessor()
	{
	}

	void ProfilerDataProcessor::Load(const std::string& a_file)
	{
		//parse
		FILE* file = fopen(a_file.c_str(), "r");

		if (file != nullptr)
		{
			fseek(file, 0, SEEK_END);
			long lSize = ftell(file);
			rewind(file);

			char* buffer = static_cast<char*>(malloc(sizeof(char) * lSize));
			size_t result = fread(buffer, 1, lSize, file);

			std::vector<std::string> strings = Split(std::string(buffer), '\n');

			printf_s("%s", strings[14]);

			free(buffer);

			fclose(file);
		}

		std::string data = "";
		Parse(data);
	}

	std::vector<std::string> Split(const std::string& a_stringToSplit, char a_splitCharacter)
	{
		std::vector<std::string> strings = std::vector<std::string>();

		int startIndex = 0;

		for (size_t i = 0; i < a_stringToSplit.length(); ++i)
		{
			if (a_stringToSplit[i] == a_splitCharacter)
			{
				// Not being deleted. Memory leak???
				char* localString = new char[i + 1];

				size_t length = a_stringToSplit.copy(localString, i - startIndex, startIndex);
				localString[length] = '\0';
				
				strings.push_back(std::string(localString));
				
				startIndex = i;

				delete[] localString;
			}
		}

		return strings;
	}

	void ProfilerDataProcessor::Parse(const std::string& a_data)
	{

	}
}