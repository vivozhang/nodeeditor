#pragma once

#include <memory>

#include <QtGui/QPainter>

namespace QtNodes
{

class ConnectionGeometry;
class ConnectionState;
class Connection;

class ConnectionPainter
{
//vivo: add point that allow user to adjust path
private:
   QPointF forcePointa;
   QPointF forcePointb;
   QPointF forcePointc;
   QPointF forcePointd;
public:

  ConnectionPainter();

public:

  static
  QPainterPath
  cubicPath(ConnectionGeometry const& geom);

  static
  QPainterPath
  straightPath(ConnectionGeometry const& geom);


  static
  QPainterPath
  getPainterStroke(ConnectionGeometry const& geom);

  static
  void
  paint(QPainter* painter,
        Connection const& connection);
};
}
