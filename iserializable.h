#ifndef ISERIALIZABLE
#define ISERIALIZABLE

#include <QJsonObject>

class ISerializable {
public:
    class ParserException{};
    virtual void read(QJsonObject settings) = 0;
    virtual QJsonObject serialize() = 0;
};

#endif // ISERIALIZABLE

