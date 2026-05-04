#ifndef SYSTEM_REGISTRY_H
#define SYSTEM_REGISTRY_H

#include <Arduino.h>
#include <vector>

enum ParamType {
    PARAM_INT,
    PARAM_FLOAT,
    PARAM_DOUBLE,
    PARAM_BOOL
};

struct ConfigParam {
    const char* name; 
    void* ptr;
    ParamType type;
};

class SystemRegistry {
private:
    std::vector<ConfigParam> params;
    SystemRegistry() {} 

public:
    static SystemRegistry& GetInstance() {
        static SystemRegistry instance;
        return instance;
    }

    void Register(const char* name, void* ptr, ParamType type) 
    {
        params.push_back({name, ptr, type});
    }

    bool SetValue(const char* name, const char* valueStr) 
    {
        for(const auto& param : params) 
        {
            if(strcmp(param.name, name) == 0) 
            {
                switch (param.type) {
                    case PARAM_INT: 
                        *(int*)param.ptr = atoi(valueStr); 
                        break;
                    case PARAM_FLOAT: 
                        *(float*)param.ptr = atof(valueStr); 
                        break;
                    case PARAM_DOUBLE: 
                        *(double*)param.ptr = atof(valueStr); 
                        break;
                    case PARAM_BOOL: 
                        *(bool*)param.ptr = (atoi(valueStr) > 0); 
                        break;
                }
                Serial.println("Value SET");
                return true;
            }
        }
        Serial.println("There is no such variable");
        return false;
    }

    void ReturnAllRegistryNames()
    {
        for(const auto& param : params)
        {
            Serial.print(param.name);
            Serial.print(": ");

            switch (param.type) {
                case PARAM_INT: 
                    Serial.println(*(int*)param.ptr); 
                    break;
                case PARAM_FLOAT: 
                    Serial.println(*(float*)param.ptr); 
                    break;
                case PARAM_DOUBLE: 
                    Serial.println(*(double*)param.ptr); 
                    break;
                case PARAM_BOOL: 
                    Serial.println(*(bool*)param.ptr); 
                    break;
                default:
                    Serial.println("UNSUPPORTED_TYPE");
                    break;
            }
        }  
    }
};

#endif