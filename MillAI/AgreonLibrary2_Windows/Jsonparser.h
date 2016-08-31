#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"
#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"
#include "Log.h"


using namespace rapidjson;

class JsonParser
{
public:
	JsonParser();
	static Document LoadJSON(const std::string& file)
	{
		Document jsondoc;

		FILE *pFile = fopen(file.c_str(), "r");
		rapidjson::FileStream is(pFile);

		jsondoc.ParseStream<0>(is);
		
		if (jsondoc.HasParseError())
		{
			AL::Log::write(AL::LOG_ERROR,"JSON Parser","parse error!!");
			return NULL;
		}

		return jsondoc;
	}
};