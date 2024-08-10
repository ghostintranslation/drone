#ifndef Motherboard_h
#define Motherboard_h

#include "Configuration.h"
#include "SystemExclusiveManager.h"

class Motherboard
{

private:
    // Singleton
    static Motherboard *instance;
    Motherboard();

public:
    static Motherboard *getInstance();
    static void init();
};

// Singleton pre init
Motherboard *Motherboard::instance = nullptr;

inline Motherboard::Motherboard()
{
}

inline void Motherboard::init()
{
    Configuration::loadFromMemory();
    SystemExclusiveManager::getInstance();
}

/**
   Singleton instance
*/
inline Motherboard *Motherboard::getInstance()
{
    if (!instance)
        instance = new Motherboard;
    return instance;
}

Motherboard *motherboard = Motherboard::getInstance();
#endif