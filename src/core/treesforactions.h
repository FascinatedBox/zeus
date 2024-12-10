#ifndef ZEUSTREEFORACTION_H
#define ZEUSTREEFORACTION_H
#include <QList>

class QTreeWidgetItem;
class ZeusBaseAction;

void addOneTreeForAction(QTreeWidgetItem *item, ZeusBaseAction *act);
void addTreesForActions(QTreeWidgetItem *item, QList<ZeusBaseAction *> act);

#endif
