#include <QString>
#include <QTextStream>
#include <QFile>
#include "nbodies.h"

NBodies::NBodies()
{
    _simulation = std::make_unique<Simulation>();
    _simulation->dt = 3600.0; // default 1 hour step time
    _integrator = std::make_unique<EulerIntegrator>();
}

void NBodies::reset()
{
    _simulation->bodies = processConfiguration();
    _bounds = QRectF();
    _updateBounds();
    emit bodiesChanged();
}

void NBodies::step()
{
    _integrator->step(*_simulation);
    _updateBounds();
    emit bodiesChanged();
}

QString NBodies::getCurrentConfiguration()
{
    return _configuration;
}

void NBodies::setConfiguration(QString &configuration)
{
    if(configuration.startsWith("qrc:/") || configuration.startsWith(":/"))
    {

        QFile file(configuration);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw std::exception();
        }
        _configuration = QString(file.readAll());
    }
    else
    {
        _configuration = configuration;
    }
    reset();
    emit configurationChanged();
}

std::vector<Body> NBodies::processConfiguration()
{
    std::vector<Body> output;

    QTextStream s(&_configuration);

    while(!s.atEnd())
    {
        QString line = s.readLine();
        if (line.isEmpty()) continue;
        if (line.startsWith("#")) continue;

        QTextStream l(&line);
        QString name;
        double mass;
        double radius;
        Vec3 position;
        Vec3 velocity;

        l >> name >> mass >>
            position[0] >> position[1] >> position[2] >>
            velocity[0] >> velocity[1] >> velocity[2] >> radius;

        output.emplace_back(name.toStdString(), position, velocity, mass, radius);
    }
    return output;
}

QList<BodyQML> NBodies::getBodies()
{
    QList<BodyQML> output;
    // scale bodies down for visualization
    double scaling = std::max(_bounds.width(), _bounds.height());
    for (auto &body : _simulation->bodies)
    {
        output.emplaceBack(body, scaling, scaling);
    }
    return output;
}


double NBodies::getStepTime()
{
    return _simulation->dt;
}

void NBodies::setStepTime(double step)
{
    _simulation->dt = step;
    emit stepTimeChanged();
}

void NBodies::_updateBounds()
{
    for (auto &body : _simulation->bodies)
    {
        if (body.position[0] < _bounds.left())
        {
            _bounds.setLeft(body.position[0]);
        }
        if (body.position[0] > _bounds.right())
        {
            _bounds.setRight(body.position[0]);
        }
        if (body.position[1] < _bounds.bottom())
        {
            _bounds.setBottom(body.position[1]);
        }
        if (body.position[1] > _bounds.top())
        {
            _bounds.setTop(body.position[1]);
        }
    }
}
