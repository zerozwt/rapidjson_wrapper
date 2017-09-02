#include <string>
#include <stdint.h>
#include <utility>
#include <vector>
#include <set>
#include <map>
#include <cstdio>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

class RapidJsonWrapper
{
public:
    static bool ParseJsonFromString(const std::string& str, rapidjson::Document& doc)
    {
        doc.Parse(str.c_str());
        if (doc.HasParseError())
        {
            return false;
        }
        return true;
    }

    static bool ParseJsonFromFile(const std::string& file_name, rapidjson::Document& doc)
    {
        FILE* f = fopen(file_name.c_str(), "r");
        if (!f)
        {
            return false;
        }

        static char read_buf[1 << 14/*16K buf*/];
        rapidjson::FileReadStream fs(f, read_buf, 1 << 14);

        doc.ParseStream(fs);

        fclose(f);

        if (doc.HasParseError())
        {
            return false;
        }
        return true;
    }

    //-----------------------------------------------------------------

    static bool GetValue(const rapidjson::Value& value, const rapidjson::Value* &out)
    {
        out = &value;
        return true;
    }

    static bool GetValue(const rapidjson::Value& value, int32_t& out)
    {
        if (value.IsInt())
        {
            out = value.GetInt();
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& value, uint32_t& out)
    {
        if (value.IsUint())
        {
            out = value.GetUint();
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& value, int64_t& out)
    {
        if (value.IsInt64())
        {
            out = value.GetInt64();
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& value, uint64_t& out)
    {
        if (value.IsUint64())
        {
            out = value.GetUint64();
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& value, bool& out)
    {
        if (value.IsBool())
        {
            out = value.IsTrue();
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& value, std::string& out)
    {
        if (value.IsString())
        {
            out.assign(value.GetString(), value.GetStringLength());
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& value, float& out)
    {
        if (value.IsFloat())
        {
            out = value.GetFloat();
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& value, double& out)
    {
        if (value.IsDouble())
        {
            out = value.GetDouble();
            return true;
        }
        return false;
    }

    template<typename T, typename A>
    static bool GetValue(const rapidjson::Value& value, std::vector<T,A>& out)
    {
        if (!value.IsArray())
        {
            return false;
        }
        for (auto it = value.Begin(); it != value.End(); ++it)
        {
            T item;
            if (GetValue(*it, item))
            {
                out.push_back(std::move(item));
            }
        }
        return true;
    }

    template<typename T, typename A>
    static bool GetValue(const rapidjson::Value& value, std::set<T,A>& out)
    {
        if (!value.IsArray())
        {
            return false;
        }
        for (auto it = value.Begin(); it != value.End(); ++it)
        {
            T item;
            if (GetValue(*it, item))
            {
                out.insert(std::move(item));
            }
        }
        return true;
    }

    template<typename K, typename V, typename C, typename A>
    static bool GetValue(const rapidjson::Value& value, std::map<K,V,C,A>& out)
    {
        if (!value.IsObject())
        {
            return false;
        }
        for (auto it = value.MemberBegin(); it != value.MemberEnd(); ++it)
        {
            K k; V v;
            if (GetValue(it->name, k) && GetValue(it->value, v))
            {
                out.insert(std::make_pair<K,V>(std::move(k), std::move(v)));
            }
        }
        return true;
    }

    //-----------------------------------------------------------------

    template<typename T>
    static bool GetValue(const rapidjson::Value& value, const std::string& key, T& out)
    {
        if (!value.IsObject())
        {
            return false;
        }
        auto it = value.FindMember(key.c_str());
        if (it == value.MemberEnd())
        {
            return false;
        }
        return GetValue(it->value, out);
    }

    //-----------------------------------------------------------------

    template<typename T>
    static T GetValueWithDefault(const rapidjson::Value& value, const std::string& key, const T& def_value)
    {
        T ret;
        if (GetValue(value, key, ret))
        {
            return std::move(ret);
        }
        return def_value;
    }

    static int32_t GetJsonInt(const rapidjson::Value& value, const std::string& key, int32_t def_value = 0)
    {
        return GetValueWithDefault(value, key, def_value);
    }

    static uint32_t GetJsonUInt(const rapidjson::Value& value, const std::string& key, uint32_t def_value = 0)
    {
        return GetValueWithDefault(value, key, def_value);
    }

    static int64_t GetJsonInt64(const rapidjson::Value& value, const std::string& key, int64_t def_value = 0)
    {
        return GetValueWithDefault(value, key, def_value);
    }

    static uint64_t GetJsonUInt64(const rapidjson::Value& value, const std::string& key, uint64_t def_value = 0)
    {
        return GetValueWithDefault(value, key, def_value);
    }

    static bool GetJsonBool(const rapidjson::Value& value, const std::string& key, bool def_value = false)
    {
        return GetValueWithDefault(value, key, def_value);
    }

    static std::string GetJsonString(const rapidjson::Value& value, const std::string& key, const std::string& def_value = "")
    {
        return GetValueWithDefault(value, key, def_value);
    }

    static float GetJsonFloat(const rapidjson::Value& value, const std::string& key, float def_value = 0.0f)
    {
        return GetValueWithDefault(value, key, def_value);
    }

    static double GetJsonDouble(const rapidjson::Value& value, const std::string& key, double def_value = 0.0)
    {
        return GetValueWithDefault(value, key, def_value);
    }

    static const rapidjson::Value* GetJsonObject(const rapidjson::Value& value, const std::string& key, const rapidjson::Value* def_value = nullptr)
    {
        return GetValueWithDefault<const rapidjson::Value*>(value, key, def_value);
    }

};