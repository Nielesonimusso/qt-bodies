#ifndef NBODIES_H
#define NBODIES_H

#include <QObject>
#include <QList>
#include <QAbstractItemModel>
#include <QRect>

#include "bodyqml.h"
#include "body.hpp"
#include "simulation.hpp"
#include "euler_integrator.hpp"

class NBodies : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString configuration READ getCurrentConfiguration WRITE setConfiguration NOTIFY configurationChanged REQUIRED)
    Q_PROPERTY(double stepTime READ getStepTime WRITE setStepTime NOTIFY stepTimeChanged)
    Q_PROPERTY(QList<BodyQML> bodies READ getBodies NOTIFY bodiesChanged)
    Q_PROPERTY(double minX READ minX NOTIFY bodiesChanged)
    Q_PROPERTY(double maxX READ maxX NOTIFY bodiesChanged)
    Q_PROPERTY(double minY READ minY NOTIFY bodiesChanged)
    Q_PROPERTY(double maxY READ maxY NOTIFY bodiesChanged)

public:
    NBodies();

    Q_INVOKABLE void reset();
    Q_INVOKABLE void step();

    QString getCurrentConfiguration();
    void setConfiguration(QString &configuration);
    std::vector<Body> processConfiguration();

    QList<BodyQML> getBodies();

    double getStepTime();
    void setStepTime(double step);

    double minX() { return _bounds.left(); }
    double maxX() { return _bounds.right(); }
    double minY() { return _bounds.bottom(); }
    double maxY() { return _bounds.top(); }

signals:
    void configurationChanged();
    void stepTimeChanged();
    void bodiesChanged();

private:
    QString _configuration;
    std::unique_ptr<Simulation> _simulation;
    std::unique_ptr<EulerIntegrator> _integrator;
    QRectF _bounds;

    void _updateBounds();
};

#endif // NBODIES_H
