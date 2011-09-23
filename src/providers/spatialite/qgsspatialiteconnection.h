/***************************************************************************
    qgsspatialiteconnection.h  -  SQLite/SpatiaLite connection
                             -------------------
    begin                : Thu Sep 22 2011
    copyright            : (C) 2011 by Giuseppe Sucameli
    email                : brush.tyler at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSSPATIALITECONNECTION_H
#define QGSSPATIALITECONNECTION_H

#include <QStringList>

class QgsSpatiaLiteProvider;

/*!
 * \brief   Connections management
 */
class QgsSpatiaLiteConnection : public QObject
{
    Q_OBJECT

  public:
    //! Constructor
    QgsSpatiaLiteConnection( QString theConnName );
    //! Destructor
    ~QgsSpatiaLiteConnection();

    static QStringList connectionList();

    static QString selectedConnection();
    static void setSelectedConnection( QString name );
    static void deleteConnection( QString name );
    static void saveConnection( QString name, QString uri );

  public:
    QgsSpatiaLiteProvider *provider();
    QString connectionInfo();
    QString mConnName;
    QString mConnectionInfo;
};


#endif // QGSSPATIALITECONNECTION_H
