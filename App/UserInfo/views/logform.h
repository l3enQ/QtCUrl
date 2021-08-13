#ifndef LOGFORM_H
#define LOGFORM_H

#include <QWidget>

namespace Ui {
class LogForm;
}

class LogForm : public QWidget
{
    Q_OBJECT

public:
    explicit LogForm(QWidget *parent = nullptr);
    ~LogForm();

    void log(const QString &message);
    void clear();

private:
    Ui::LogForm *ui;
};

#endif // LOGFORM_H
