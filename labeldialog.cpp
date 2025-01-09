#include "labeldialog.h"
#include "qlistwidget.h"
#include "qmenu.h"
#include "ui_labeldialog.h"
#include "idatabase.h"

// 构造函数：初始化标签对话框，设置UI元素和连接
LabelDialog::LabelDialog(QWidget *parent, QString filepath) : QDialog(parent), ui(new Ui::LabelDialog)
{
    ui->setupUi(this);
    setWindowTitle("书签");
    setMinimumSize(300, 400);

    this->filepath = filepath;
    isChange = false;

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->listWidget, &QListWidget::itemClicked, this, &LabelDialog::onItemClicked);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested, this, &LabelDialog::onCustomContextMenuRequested);
}

void LabelDialog::loadBookmarksFromDatabase()
{
    QList<QMap<QString, QVariant>> labels = IDataBase::getInstance().getLabelsByFilePath(filepath);
    for (const auto &label : labels)
    {
        int row = label["row"].toInt();
        QString message = label["message"].toString();

        addBookmark(row, message); // 添加书签
        bookmarkRows.insert(row);      // 添加行号
    }
}

// 向列表控件添加新书签，包含行号和消息
void LabelDialog::addBookmark(int row, const QString &message)
{
    isChange = true;
    QString itemText = QString("行 %1: %2").arg(row).arg(message);
    QListWidgetItem *item = new QListWidgetItem(itemText, ui->listWidget);

    item->setData(Qt::UserRole - 1, row);
    item->setData(Qt::UserRole - 2, message);
    bookmarkRows.insert(row);
}

// 通过行号从列表控件中删除书签
void LabelDialog::removeBookmark(int row)
{
    isChange = true;
    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        if (item->data(Qt::UserRole - 1).toInt() == row)
        {
            ui->listWidget->removeItemWidget(item);
            delete item;
            break;
        }
    }
    bookmarkRows.remove(row);
}

void LabelDialog::save()
{
    if (!isChange)
    {
        return;
    }
    IDataBase::getInstance().deleteAllLabels(filepath);
    for (int i = 0 ; i < ui->listWidget->count(); i++)
    {
        IDataBase::getInstance().addLabel(filepath, ui->listWidget->item(i)->data(Qt::UserRole - 1).toInt(), ui->listWidget->item(i)->data(Qt::UserRole - 2).toString());
    }
    isChange = false;
}

bool LabelDialog::isRowBookmarked(int row)
{
    return bookmarkRows.contains(row);
}

// 处理项目点击事件并发出信号以跳转到选定行
void LabelDialog::onItemClicked(QListWidgetItem *item)
{
    emit jumpToLineRequested(item->data(Qt::UserRole - 1).toInt());
}

// 处理书签项目的右键上下文菜单
void LabelDialog::onCustomContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = ui->listWidget->itemAt(pos);
    if (!item)
        return;

    QMenu menu(this);
    QAction *deleteAction = menu.addAction("删除书签");
    connect(deleteAction, &QAction::triggered, this, [this, item]()
            { this->removeBookmark(item->data(Qt::UserRole - 1).toInt()); });

    menu.exec(ui->listWidget->mapToGlobal(pos));
}
