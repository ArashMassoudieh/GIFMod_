#pragma once
#include "qlineedit.h"
#include "qcombobox.h"
#include "qlistview.h"
#include "XString.h"
#include <QDoubleValidator>
#include <QHBoxLayout>

class UnitTextBox3 :
	public QWidget
{	
//	Q_OBJECT
public:
    UnitTextBox3() {}

	UnitTextBox3(const XString &X, QWidget * parent = 0) :QWidget(parent)
	{
		QHBoxLayout *layout = new QHBoxLayout(this);
		layout->setMargin(0);

		textBox = new QLineEdit(this);
		layout->addWidget(textBox);
		
		unitBox = new QComboBox(textBox);
		layout->addWidget(unitBox);
		layout->setSizeConstraint(QLayout::SetMaximumSize);
		setLayout(layout);
		validator = new QDoubleValidator(this);
		textBox->setValidator(validator);
        setGeometry(QRect(0, 0, 300, 20));
		setXString(X);
		setFocusProxy(textBox);
    }

	UnitTextBox3(const QStyleOptionViewItem &option, QWidget * parent = 0) :QWidget(parent)
	{
		textBox = new QLineEdit(this);
		unitBox = new QComboBox(textBox);
		validator = new QDoubleValidator(this);
		textBox->setValidator(validator);
        setGeometry(option.rect);
		unitBox->show();
		textBox->show();
		this->show();
    }

    ~UnitTextBox3(){}

	void paintEvent(QPaintEvent * event)
	{
		unitBox->show();
		textBox->show();
		this->show();

		textBox->resize(rect().width(), rect().height());
		QFont QF = font(); QF.setPointSize(QF.pointSize() - 1);
		unitBox->setFont(QF);
		unitBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		int w = (unitBox->rect().width() < rect().width() / 2) ? unitBox->rect().width() : rect().width() / 2;
		unitBox->setGeometry(rect().width() - unitBox->rect().width(), 0, w, rect().height());
    }

    void setXString(const XString &X)
	{ 
		setText(X.toQString()); 
		setUnitsList(X.unitsList); 
		setUnit(X.unit);
		defaultUnit = X.defaultUnit;

    }

    void setText(const QString &T){ textBox->setText(T); }
    void setUnit(const QString &U){ unitBox->setCurrentText(U); }
    void setUnitsList(const QStringList &L){ for (int i = 0; unitBox->count(); i++) unitBox->clear(); unitBox->addItems(L); }
    QString text() const { return textBox->text(); }
    QString unit() const { return unitBox->currentText(); }
    QStringList units() const { QStringList R; for (int i = 0; i < unitBox->count(); i++) R.append(unitBox->itemText(i)); return R; }
    QStringList list() const { return QStringList() << text() << unit() << units() << defaultUnit; }
    QRect rect() const { return geometry(); }
	QString defaultUnit;

	XString toXString() {
		XString(list());
	}

private:
	QComboBox *unitBox;
    QLineEdit *textBox;
	QDoubleValidator *validator;
	};

