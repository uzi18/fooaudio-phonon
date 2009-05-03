#ifndef _FOOPLUGINDIALOG_HPP_
#define _FOOPLUGINDIALOG_HPP_

#include <QDialog>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class FooPluginDialog : public QDialog
{
	Q_OBJECT

public:
	FooPluginDialog(const QString &path, const QStringList &fileNames, QWidget *parent = 0);

private:
	QLabel *label;
	QTreeWidget *treeWidget;
	QPushButton *okButton;
	QIcon interfaceIcon;
	QIcon featureIcon;

	void findPlugins(const QString &path, const QStringList &fileNames);
	void populateTreeWidget(QObject *plugin, const QString &text);
	void addItems(QTreeWidgetItem *pluginItem, const char *interfaceName, const QStringList &features);
};

#endif //_FOOPLUGINDIALOG_HPP_

