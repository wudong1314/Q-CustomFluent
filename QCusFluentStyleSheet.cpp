#include "QCusFluentStyleSheet.h"
#include <QFontDatabase>
#include <QFile>
#include "QDebug"

QString QCusFluenStyleSheet::defaultStyleSheet()
{
    QString qss;
    const QStringList qssFiles{ { ":/stylesheet/Resources/stylesheet/default-light.css" },
                                 { ":/stylesheet/Resources/stylesheet/sw-light.css" } };

    QFontDatabase fontDatabase;

    // 2. 获取所有可用的字体族名称
    // families() 返回一个 QStringList，其中包含了系统中所有字体族的名称
    QString strFont = "DINCond-Bold";
    QStringList fontFamilies = fontDatabase.families();
    bool isDIN = true;
    if (fontFamilies.indexOf(strFont) < 0)
    {
        strFont = "Microsoft YaHei";
        isDIN = false;
    }

    for (const auto& qssFile : qssFiles)
    {
        QFile file(qssFile);
        if (file.open(QFile::ReadOnly))
        {
            QByteArray ss = file.readAll();
            if (!isDIN)
            {
                ss.replace(strFont, "Microsoft YaHei");
            }
            qss.append(ss);
        }
    }
    return qss;
}