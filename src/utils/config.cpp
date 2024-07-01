#include "config.h"

#include <QSettings>

Config::~Config()
{
    for (auto it = m_hash.begin(); it != m_hash.end(); it++)
    {
        this->setValue(it.key(), *it);
    }
}

void Config::reg(std::initializer_list<std::pair<QString, Item>> list)
{
    for (auto& [key, item] : list)
    {
        if (m_settings.contains(key))
        {
            auto v = this->value(key);
            if (item.m_check(v))
            {
                auto it = item;
                it.QVariant::operator=(v);
                m_hash.insert(key, it);
                continue;
            }
        }
        this->setValue(key, item);
        m_hash.insert(key, item);
    }
}

