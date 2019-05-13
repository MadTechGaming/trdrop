#ifndef FILEITEMMODEL_H
#define FILEITEMMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include "headers/fileitem.h"

class FileItemModel : public QAbstractListModel
{
    Q_OBJECT

//! constructors
public:
    //!
    FileItemModel(QObject * parent = nullptr)
        : QAbstractListModel(parent)
    {
        _setup_role_names();
        appendDefaultFileItem();
        appendDefaultFileItem();
        appendDefaultFileItem();
    }
    //!
    enum FileItemRoles
    {
        NameRole         = Qt::UserRole
      , SizeMBRole       = Qt::UserRole + 1
      , PositionRole     = Qt::UserRole + 2
      , ContainerRole    = Qt::UserRole + 3
      , FileSelectedRole = Qt::UserRole + 4
    };

//! methods
public:
    //! number of elements in the _file_item_list which correlate to the rows
    int rowCount(const QModelIndex & parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return _file_item_list.size();
    }
    //! a getter for the _role_names to enable the access via QML
    QHash<int, QByteArray> roleNames() const override { return _role_names; }
    //! TODO
    Q_INVOKABLE QVariant isFileSelected(int index) { return QVariant::fromValue(_file_item_list.at(index).fileSelected()); }
    //! TODO
    Q_INVOKABLE void remove(int index)
    {
        emit beginRemoveRows(QModelIndex(), index, index);
        _file_item_list.removeAt(index);
        emit endRemoveRows();
    }
    //!
    QVariant data(const QModelIndex & index,int role) const override
    {
        int row = index.row();
        // if the index is out of bounds, return QVariant
        if(row < 0 || row >= _file_item_list.size()) { return QVariant(); }
        // otherwise get the item
        const FileItem & file_item = _file_item_list.at(row);
        // check which member is accessed and return accordingly
        switch (role)
        {
            case NameRole:
                return file_item.name();
            case SizeMBRole:
                return file_item.sizeMB();
            case PositionRole:
                return file_item.position();
            case ContainerRole:
                return file_item.container();
            case FileSelectedRole:
                return file_item.fileSelected();
            default:
                return QVariant();
        }
    }
    //!
    bool setData(const QModelIndex & index, const QVariant & value, int role) override
    {
        FileItem & file_item = _file_item_list[index.row()];
        if (role == NameRole) file_item.setName(value.toString());
        else if (role == SizeMBRole) file_item.setSizeMB(value.toUInt());
        else if (role == PositionRole) file_item.setPosition(static_cast<quint8>(value.toUInt()));
        else if (role == ContainerRole) file_item.setContainer(value.toString());
        else if (role == FileSelectedRole) file_item.setFileSelected(value.toBool());
        else return false;
        emit dataChanged(index, index);
        return true ;
    }
    //! tells the views that the model's state has changed -> this triggers a "recompution" of the delegate
    Q_INVOKABLE void resetModel()
    {
        beginResetModel();
        endResetModel();
    }
    //! adds a default fileitem
    Q_INVOKABLE void appendDefaultFileItem()
    {
        const FileItem file_item;
        _append_file_item(file_item);
    }

//! methods
private:
    //! Set names to the role name hash container (QHash<int, QByteArray>)
    //! model.name, model.sizeMB, model.position, model.container, model.fileSelected
    void _setup_role_names()
    {
        _role_names[NameRole] = "name";
        _role_names[SizeMBRole] = "sizeMB";
        _role_names[PositionRole] = "position";
        _role_names[ContainerRole] = "container";
        _role_names[FileSelectedRole] = "fileSelected";
    }
    //! add file item object
    void _append_file_item(const FileItem file_item)
    {
        int new_row = rowCount();
        emit beginInsertRows(QModelIndex(), new_row, new_row);
        _file_item_list.append(file_item);
        emit endInsertRows();
    }
//! member
private:
    //! TODO
    QList<FileItem> _file_item_list;
    //! TODO
    QHash<int, QByteArray> _role_names;
};

#endif // FILEITEMMODEL_H
