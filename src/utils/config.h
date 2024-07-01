#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>

#define CFG Config::get

class Config
{
    Config(QSettings& settings) : m_settings(settings) { }
public:
    ~Config();

    static Config& get()
    {
        static QSettings s(CONFIG_INI, QSettings::IniFormat);
        static Config c(s);
        return c;
    }

    class Item : public QVariant
    {
        friend class Config;
    public:
        Item() : Item(QVariant()) { }
        Item(const QVariant& v, std::function<bool(QVariant)> f = [](auto) { return true; })
            : QVariant(v), m_d_value(v), m_check(f)
        { Q_ASSERT(m_check(v)); }

        const QVariant defaultValue() const { return m_d_value; }

        Item& operator=(const QVariant& v)
        {
            if (m_check(v))
            {
                QVariant::operator=(v);
            }
            return *this;
        }

    private:
        QVariant m_d_value; // default value
        std::function<bool(QVariant)> m_check;
    };

    void reg(std::initializer_list<std::pair<QString, Item>> list); // register

    QVariant operator[](const QString& key) const
    {
        Q_ASSERT_X(m_hash.find(key) != m_hash.end(), "Config::operator[]", "item cannot register");
        return m_hash[key];
    }

    Item& operator[](const QString& key)
    {
        Q_ASSERT_X(m_hash.find(key) != m_hash.end(), "Config::operator[]", "item cannot register");
        return m_hash[key];
    }

    QVariant operator()(QAnyStringView key) const { return m_settings.value(key); }

    QVariant value(QAnyStringView key) const { return (*this)(key); }

    void setValue(QAnyStringView key, QVariant value)
    { m_settings.setValue(key, value); }

private:
    QSettings& m_settings;
    QHash<QString, Item> m_hash;
};

#endif // CONFIG_H
