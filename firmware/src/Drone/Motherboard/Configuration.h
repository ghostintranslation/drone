#ifndef Configuration_h
#define Configuration_h

#include "Setting.h"
#include <EEPROM.h>
#include "lib/ArduinoStreamUtils/StreamUtils.h"

class Configuration
{

private:
public:
    static void loadFromMemory();
    static void loadFromJson(JsonDocument doc);
    static void save();
    static JsonDocument getJson(bool onlyIdandValue);
};

// TODO:
inline void Configuration::loadFromMemory()
{
    Serial.println("Configuration::loadFromMemory");
    Serial.println(EEPROM.length());

    // Find EOF index to know the length
    uint16_t eofIndex = 0;
    for (int i = 0; i < EEPROM.length(); i++)
    {
        Serial.print((char)EEPROM.read(i));
        if (EEPROM.read(i) == 26)
        {
            eofIndex = i;
            break;
        }
    }
    Serial.println("");

    JsonDocument doc;
    EepromStream eepromStream(0, eofIndex + 1);

    DeserializationError error = deserializeJson(doc, eepromStream);

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }

    Configuration::loadFromJson(doc);
}

inline void Configuration::loadFromJson(JsonDocument doc)
{
    Serial.println("Configuration::loadFromJson");
    JsonArray settings = doc["settings"];
    for (JsonVariant item : settings)
    {
        for (unsigned int i = 0; i < Setting::getCount(); i++)
        {
            Setting *setting = Setting::get(i);
            // Serial.println((String)item["id"]);
            // Serial.println(setting->get(i)->getId());
            // Serial.println("");
            if (item["id"] == setting->get(i)->getId())
            {
                // Serial.println("Configuration::loadFromJson 2");
                setting->setValue(item["value"]);
            }
        }
    }
}

// TODO:
inline void Configuration::save()
{
    Serial.println("Configuration::save");
    Serial.println(EEPROM.length());

    EepromStream eepromStream(0, EEPROM.length());

    JsonDocument doc = Configuration::getJson(true);
    uint16_t length = serializeJson(doc, eepromStream);
    Serial.printf("Serialized length: %d", length);
    Serial.println("");
    EEPROM.write(length, 26); // End of file char
}

inline JsonDocument Configuration::getJson(bool onlyIdandValue = false)
{
    JsonDocument doc;

    for (unsigned int i = 0; i < Setting::getCount(); i++)
    {
        doc["settings"][i] = Setting::get(i)->serialize(onlyIdandValue);
    }

    doc.shrinkToFit();

    return doc;
}
#endif