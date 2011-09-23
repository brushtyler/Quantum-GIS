/***************************************************************************
    qgsspatialiteconnection.cpp  -  SQLite/SpatiaLite connection
                             -------------------
    begin                : 22 June 2011
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

#include "qgsspatialiteconnection.h"

#include <qgslogger.h>
#include "qgsspatialiteprovider.h"
#include "qgsproviderregistry.h"
#include "qgsdatasourceuri.h"

#include <QSettings>

QStringList QgsSpatiaLiteConnection::connectionList()
{
  QSettings settings;
  settings.beginGroup( "/SpatiaLite/connections" );
  QStringList connList;
  foreach ( QString name, settings.childGroups() )
  {
    QString text = name + tr( "@" );
    text += settings.value( name + "/sqlitepath", "###unknown###" ).toString();
    connList << text;
  }
  settings.endGroup();
  return connList;
}

QString QgsSpatiaLiteConnection::selectedConnection()
{
  QSettings settings;
  return settings.value( "/SpatiaLite/connections/selected" ).toString();
}

void QgsSpatiaLiteConnection::setSelectedConnection( QString name )
{
  QSettings settings;
  return settings.setValue( "/SpatiaLite/connections/selected", name );
}

void QgsSpatiaLiteConnection::deleteConnection( QString name )
{
  QSettings settings;
  QString subKey = QString( name );
  int idx = subKey.indexOf( "@" );
  if ( idx > 0 )
    subKey.truncate( idx );

  QString key = "/SpatiaLite/connections/" + subKey;
  settings.remove( key + "/sqlitepath" );
  settings.remove( key );
}

void QgsSpatiaLiteConnection::saveConnection( QString name, QString uri )
{
  QString sqlitePath;
  if ( uri.startsWith( "dbname='" ) )
  {
    sqlitePath = QgsDataSourceURI( uri ).database();
  }
  else
  {
    sqlitePath = uri;
  }
  QSettings settings;
  QString key = "/SpatiaLite/connections/" + name;
  settings.setValue( key + "/sqlitepath", sqlitePath );
}


QgsSpatiaLiteConnection::QgsSpatiaLiteConnection( QString theConnName )
{
  QgsDebugMsg( "theConnName = " + theConnName );

  // retrieving the SQLite DB name and full path
  mConnName = QString( theConnName );
  int idx = mConnName.indexOf( "@" );
  if ( idx > 0 )
    mConnName.truncate( idx );

  QSettings settings;
  QString key = "/SpatiaLite/connections/" + mConnName;

  QString sqlitePath = settings.value( key + "/sqlitepath", "###unknown###" ).toString();
  mConnectionInfo = QString( "dbname='%1'" ).arg( QString( sqlitePath ).replace( "'", "\\'" ) );

  QgsDebugMsg( QString( "Connection info: '%1'." ).arg( mConnectionInfo ) );
}

QgsSpatiaLiteConnection::~QgsSpatiaLiteConnection()
{

}

QString QgsSpatiaLiteConnection::connectionInfo( )
{
  return mConnectionInfo;
}

QgsSpatiaLiteProvider * QgsSpatiaLiteConnection::provider( )
{
  // TODO: Create and bind to data provider

  // load the server data provider plugin
  QgsProviderRegistry * pReg = QgsProviderRegistry::instance();

  QgsSpatiaLiteProvider *spatialiteProvider =
    ( QgsSpatiaLiteProvider* ) pReg->provider( "spatialite", mConnectionInfo );

  return spatialiteProvider;
}

