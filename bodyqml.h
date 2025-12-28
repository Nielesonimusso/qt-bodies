#ifndef BODYQML_H
#define BODYQML_H

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>
#include <QDebug>

#include "body.hpp"

struct BodyQML {
    Q_GADGET

    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(double mass READ getMass WRITE setMass)
    Q_PROPERTY(double radius READ getRadius WRITE setRadius)

    Q_PROPERTY(double x READ getX WRITE setX)
    Q_PROPERTY(double y READ getY WRITE setY)
    Q_PROPERTY(double z READ getZ WRITE setZ)

    Q_PROPERTY(double dx READ getDX)
    Q_PROPERTY(double dy READ getDY)

    Q_PROPERTY(double vx READ getVX WRITE setVX)
    Q_PROPERTY(double vy READ getVY WRITE setVY)
    Q_PROPERTY(double vz READ getVZ WRITE setVZ)

    QML_VALUE_TYPE(body)

public:
    BodyQML() : _body("", {0,0,0}, {0,0,0}, 0, 0)
    {
    }
    BodyQML(const Body &body, double scaleX = 1.0, double scaleY = 1.0) :
        _body(body), _scaleX(scaleX), _scaleY(scaleY)
    {
    }
    QString getName() { return QString::fromStdString(_body.name); }
    double getMass() { return _body.mass; }
    double getRadius() { return _body.radius; }

    double getX() { return _body.position[0]; }
    double getY() { return _body.position[1]; }
    double getZ() { return _body.position[2]; }
    double getDX() { return _body.position[0] / _scaleX; }
    double getDY() { return _body.position[1] / _scaleY; }
    double getVX() { return _body.velocity[0]; }
    double getVY() { return _body.velocity[1]; }
    double getVZ() { return _body.velocity[2]; }

    void setName(QString &name) { _body.name = name.toStdString(); }
    void setMass(double mass) { _body.mass = mass; }
    void setRadius(double radius) { _body.radius = radius; }

    void setX(double x) { _body.position[0] = x; }
    void setY(double y) { _body.position[1] = y; }
    void setZ(double z) { _body.position[2] = z; }
    void setVX(double vx) { _body.velocity[0] = vx; }
    void setVY(double vy) { _body.velocity[1] = vy; }
    void setVZ(double vz) { _body.velocity[2] = vz; }

private:
    double _scaleX = 1.0;
    double _scaleY = 1.0;
    Body _body;
};



#endif // BODYQML_H
