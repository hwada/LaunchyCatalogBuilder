#include <iostream>
#include <tchar.h>
// Qt 4.7
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QSettings>
#include <QDir>
// Everything 1.2.1.371
// http://www.voidtools.com/
// http://support.voidtools.com/everything/SDK
#include "catalog.h"
#include "catalog_types.h"
#include "directory.h"
#include "everything.h"

#ifdef _DEBUG
#pragma comment(lib, "QtMaind.lib")
#pragma comment(lib, "QtCored4.lib")
#pragma comment(lib, "QtCored4.lib")
#else
#pragma comment(lib, "QtMain.lib")
#pragma comment(lib, "QtCore4.lib")
#pragma comment(lib, "QtCore4.lib")
#endif

void write(char const* search, QDataStream& out)
{
	Everything_Reset();
	Everything_SetRegex(TRUE);
	Everything_SetSearchA(search);
	Everything_QueryA(TRUE);

	for (int i = 0; i < Everything_GetNumFileResults(); ++i) {
		QString const path = QString::fromUtf8(Everything_GetResultPathA(i));
		QString const file = QString::fromUtf8(Everything_GetResultFileNameA(i));
		out << CatItem(path + "/" + file);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc <= 1)
	{
		return -1;
	}

	QByteArray ba;
	QDataStream out(&ba, QIODevice::ReadWrite); 
	out.setVersion(QDataStream::Qt_4_2);

	write("\\.exe$", out);
	write("\\.lnk$", out);

	// Compress and write the catalog to the specified file
	QString const filename = argv[1];
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		qWarning("Could not open catalog file for writing");
		return false;
	}
	file.write(qCompress(ba));

	return 0;
}

