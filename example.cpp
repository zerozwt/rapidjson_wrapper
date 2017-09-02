#include <iostream>
#include "rapidjson_wrapper.h"

int main()
{
    { //Test case 1
        std::string json = "{\"key1\":12345,\"key2\":\"some string content\",\"key3\":[1,2,3,4,5]}";
        rapidjson::Document doc;
        if (RapidJsonWrapper::ParseJsonFromString(json, doc))
        {
            int value1 = 0;
            if (RapidJsonWrapper::GetValue(doc, "key1", value1))
            {
                std::cout << "GetValue key1 = " << value1 << std::endl;
            }
            std::cout << "GetJsonInt key1 = " << RapidJsonWrapper::GetJsonInt(doc, "key1") << std::endl;

            std::string value2;
            if (RapidJsonWrapper::GetValue(doc, "key2", value2))
            {
                std::cout << "GetValue key2 = " << value2 << std::endl;
            }
            std::cout << "GetJsonString key2 = " << RapidJsonWrapper::GetJsonString(doc, "key2") << std::endl;

            std::vector<int> value3;
            if (RapidJsonWrapper::GetValue(doc, "key3", value3))
            {
                std::cout << "GetValue key3 =";
                for (int& i : value3)
                {
                    std::cout << " " << i; 
                }
                std::cout << std::endl;
            }
        }
    }

    { //Test case 2 : containers (vector/set/map) can be used together
        std::string json = "{\"key\":{\"sub_key1\":[1,2,3,4,5],\"sub_key2\":[9,8,7,6,5]}}";
        rapidjson::Document doc;
        if (RapidJsonWrapper::ParseJsonFromString(json, doc))
        {
            std::map<std::string, std::vector<int>> value;
            if (RapidJsonWrapper::GetValue(doc, "key", value))
            {
                std::cout << "--------------\n";
                for (auto it = value.begin(); it != value.end(); ++it)
                {
                    std::cout << it->first << " :";
                    for (int& i : it->second)
                    {
                        std::cout << " " << i;
                    }
                    std::cout << std::endl;
                }
            }
        }
    }
    return 0;
}
