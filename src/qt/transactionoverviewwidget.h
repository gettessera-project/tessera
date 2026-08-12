// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_QT_TRANSACTIONOVERVIEWWIDGET_H
#define TESSERA_QT_TRANSACTIONOVERVIEWWIDGET_H

#include <QListView>
#include <QSize>

QT_BEGIN_NAMESPACE
class QShowEvent;
class QWidget;
QT_END_NAMESPACE

class TransactionOverviewWidget : public QListView
{
    Q_OBJECT

public:
    explicit TransactionOverviewWidget(QWidget* parent = nullptr);
    QSize sizeHint() const override;

protected:
    void showEvent(QShowEvent* event) override;
};

#endif // TESSERA_QT_TRANSACTIONOVERVIEWWIDGET_H
