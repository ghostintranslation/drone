#ifndef Setting_h
#define Setting_h

#include "Registrar.h"
#include "lib/ArduinoJson-v7.1.0.h"

class Setting : public Registrar<Setting>
{
public:
    Setting(String id, String name, float defaultValue, float min, float max, float step);
    String getId();
    String getName();
    float getDefaultValue();
    float getValue();
    float getMin();
    float getMax();
    float getStep();
    void setValue(float value);
    JsonDocument serialize(bool onlyIdandValue);

private:
    String id;
    String name;
    float defaultValue = NAN;
    float value = NAN;
    float min = NAN;
    float max = NAN;
    float step = NAN;
};

inline Setting::Setting(String id, String name, float defaultValue, float min, float max, float step)
{
    this->id = id;
    this->name = name;
    this->defaultValue = defaultValue;
    this->min = min;
    this->max = max;
    this->step = step;
}

inline String Setting::getId()
{
    return this->id;
}

inline String Setting::getName()
{
    return this->name;
}

inline float Setting::getValue()
{
    return this->value;
}

inline float Setting::getDefaultValue()
{
    return this->defaultValue;
}

inline float Setting::getMin()
{
    return this->min;
}

inline float Setting::getMax()
{
    return this->max;
}

inline float Setting::getStep()
{
    return this->step;
}

inline void Setting::setValue(float value)
{
    this->value = value;
}

inline JsonDocument Setting::serialize(bool onlyIdandValue = false)
{
    JsonDocument doc;

    // create an object
    JsonObject setting = doc.to<JsonObject>();
    setting["id"] = this->getId();
    setting["value"] = this->getValue();
    if (!onlyIdandValue)
    {
        setting["name"] = this->getName();
        setting["defaultValue"] = this->getDefaultValue();
        setting["min"] = this->getMin();
        setting["max"] = this->getMax();
        setting["step"] = this->getStep();
    }
    return doc;
}

#endif