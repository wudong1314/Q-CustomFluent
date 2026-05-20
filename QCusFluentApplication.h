#pragma once

#include <QApplication>

#include <memory>

class QCusFluentApplication : public QApplication
{
    Q_OBJECT

public:
    QCusFluentApplication(int& argc, char** argv, int = ApplicationFlags, bool defaultStyle = true);
    ~QCusFluentApplication();

    bool checkOtherProcess(const QString& key);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
