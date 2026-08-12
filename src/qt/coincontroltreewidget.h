// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_QT_COINCONTROLTREEWIDGET_H
#define TESSERA_QT_COINCONTROLTREEWIDGET_H

#include <QKeyEvent>
#include <QTreeWidget>

class CoinControlTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit CoinControlTreeWidget(QWidget *parent = nullptr);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // TESSERA_QT_COINCONTROLTREEWIDGET_H
