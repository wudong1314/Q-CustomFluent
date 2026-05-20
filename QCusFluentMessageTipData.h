#pragma once

#include <QDateTime>
#include <QString>

#include <memory>

class QCusFluentAbstractMessageTipData
{
public:
    QCusFluentAbstractMessageTipData();
    virtual ~QCusFluentAbstractMessageTipData();
};

class QCusFluentMessageTipData : public QCusFluentAbstractMessageTipData
{
public:
    enum Column
    {
        Title,
        Content,
        Time,
        ColumnCount
    };

    QCusFluentMessageTipData(const QString& title, const QString& content, const QDateTime& time);
    ~QCusFluentMessageTipData();

    void setTitle(const QString& title);
    const QString& title() const;
    void setContent(const QString& content);
    const QString& content() const;
    void setDateTime(const QDateTime& time);
    const QDateTime& dateTime() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
