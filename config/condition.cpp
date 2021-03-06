#include "condition.h"
#include "configmanager.h"

#include <QDebug>
#include <QRegularExpression>

QString Condition::cond() const
{
    return d->_cond;
}

void Condition::setCond(const QString &cond)
{
    d->_cond = cond;
}

QStringList Condition::requirement() const
{
    return d->requirement;
}

bool Condition::result() const
{
    return d->finalResult;
}

Condition::Condition() : d(new ConditionData)
{

}

Condition::Condition(const Condition &other) : d(other.d)
{
}

Condition::Condition(const QString &cond, const ConfigManager *config)
    : d(new ConditionData(cond, config))
{
}

Condition &Condition::operator =(const Condition &other)
{
    d = other.d;
    return *this;
}

Condition &Condition::operator =(Condition &&other)
{
    d = other.d;
    return *this;
}

bool Condition::check()
{
    if (d->_cond.isEmpty())
        return true;
    QString cond;

    std::function<QString(QString, QString)> checkRule = [this](QString name, QString value) -> QString {
        QString val = "";
        if (name.isEmpty() || value.isEmpty())
            return QString();

        if (name == "features") {
            auto state = d->_config->featureState(value);

            if (state == Qt::Unchecked)
                val = "0";
            else
                val = "1";
            d->results.insert("Feature " + value, val == "1");
            d->requirement.append(value);
        }
        if (name == "config") {
            val = d->_config->hasConfig(value) ? "1" : "0";
            d->results.insert("Config " + value, val == "1");
        }
        if (name == "test") {
            val = "1";
        }
        return val;
    };

     QString name, value;
     bool lastIsLetter = false;
     int step = 0; // none, name, value, etc

     for (int i = 0; i < d->_cond.length(); ++i) {
         auto ch = d->_cond.at(i);
         if (ch.isSpace())
             continue;

         if (ch == ".") {
             step++;
             continue;
         }
         if (ch.isLetter() || ch.isDigit() || ch == '_' || ch == '-') {
             if (!lastIsLetter)
                 step++;
             if (step == 1)
                 name.append(ch);
             else if (step == 2)
                 value.append(ch);
             lastIsLetter = true;
         } else {
             cond.append(checkRule(name, value));
             cond.append(ch);
             step = 0;
             name = value = "";
             lastIsLetter = false;
         }
     }
     cond.append(checkRule(name, value));

     QString cond2 = cond;
     forever{
        cond = cond
                .replace("1&&1", "1")
                .replace("1&&0", "0")
                .replace("0&&1", "0")
                .replace("0&&0", "0")

                .replace("1||1", "1")
                .replace("1||0", "1")
                .replace("0||1", "1")
                .replace("1||0", "0")

                .replace("(1)", "1")
                .replace("(0)", "0")

                .replace("!1", "0")
                .replace("!0", "1");
         if (cond2 == cond)
             break;
         cond2 = cond;
     }

     d->finalResult = cond == "1";
     return d->finalResult;
}

QString Condition::toString() const
{
    QString s;

    auto i = d->results.begin();
    while (i != d->results.end()) {
        if (!s.isEmpty())
            s.append("\n");
        s.append(QString("%1: %2")
                 .arg(i.key())
                 .arg(i.value() ? "true" : "false"));
        ++i;
    }
    return s;
}
