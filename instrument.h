#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QString>

class Instrument
{
public:
    enum Type {
        BasicWoodenFish,
        ElectronicIncense,
        TurboWoodenFish,
        QuantumPrayerBeads,
        AIChantingMachine
    };

    Instrument(Type type = BasicWoodenFish);

    QString name() const;
    double price() const;
    double clickReward() const;
    double autoReward() const;
    double maintenanceCost() const;
    double critChance() const;
    int duration() const;

    Type type() const;
    bool hasCrit() const;
    bool hasAutoReward() const;
    bool hasMaintenanceCost() const;

    QString unstrikedImagePath() const;
    QString strikedImagePath() const;
    bool isLotusInstrument() const;
    bool isCloudInstrument() const;

    static QString typeToString(Type type);
    static Type stringToType(const QString& str);

private:
    Type m_type;
};

#endif // INSTRUMENT_H
