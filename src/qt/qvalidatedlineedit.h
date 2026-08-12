// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_QT_QVALIDATEDLINEEDIT_H
#define TESSERA_QT_QVALIDATEDLINEEDIT_H

#include <QLineEdit>

/** Line edit that can be marked as "invalid" to show input validation feedback. When marked as invalid,
   it will get a red background until it is focused.
 */
class QValidatedLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit QValidatedLineEdit(QWidget *parent);
    void clear();
    void setCheckValidator(const QValidator *v);
    bool isValid();

protected:
    void focusInEvent(QFocusEvent *evt) override;
    void focusOutEvent(QFocusEvent *evt) override;

private:
    bool valid{true};
    const QValidator* checkValidator{nullptr};

public Q_SLOTS:
    void setText(const QString&);
    void setValid(bool valid);
    void setEnabled(bool enabled);

Q_SIGNALS:
    void validationDidChange(QValidatedLineEdit *validatedLineEdit);

private Q_SLOTS:
    void markValid();
    void checkValidity();
};

#endif // TESSERA_QT_QVALIDATEDLINEEDIT_H
