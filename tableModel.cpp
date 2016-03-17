#include "tableModel.h"
#include "global.h"

TableModel::TableModel( QObject *parent /*= NULL*/ )
	:QAbstractTableModel(parent)
{

}

TableModel::~TableModel()
{

}

int TableModel::rowCount( const QModelIndex &parent /*= QModelIndex()*/ ) const
{
	return tableItems.count();
}

int TableModel::columnCount( const QModelIndex &parent /*= QModelIndex()*/ ) const
{
	return 4;
}

QVariant TableModel::data( const QModelIndex &index, int role /*= Qt::DisplayRole */ )const
{
	int row = index.row();
	int column = index.column();
	if (role == Qt::DisplayRole)
	{
		switch(column)
		{
		case 0:
			return tableItems.at(row).fileName;
		case 1:
			return tableItems.at(row).fileSize;
		case 2:
			return tableItems.at(row).cpoyTime;
		case 3:
			return tableItems.at(row).speedPerMB;
		}
	}
	return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (orientation == Qt::Horizontal)
	{
		if (role == Qt::DisplayRole)
		{
			switch(section)
			{
			case 0:
				return "文件名";
			case 1:
				return "文件大小(B)";
			case 2:
				return "拷贝时长(s)";
			case 3:
				return "拷贝速度(s/M)";
			}
		}
	}
	return QVariant();
}

void TableModel::clearTableData()
{
	tableItems.clear();
	reset();
}

void TableModel::addItem( TableData tableItem)
{
	tableItems.push_back(tableItem);
	reset();
}
