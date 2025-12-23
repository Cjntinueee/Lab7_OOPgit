#include <ConcrFactory.h>
#include <Ellipse.h>
#include <Rectangle.h>
#include <Triangle.h>
#include <Group.h>

CFigure* ConcrFactory::create(const std::string& type) {
    if (type == "Rectangle") {
        return new CRect(0, 0, 1, 1);
    }
    if (type == "Ellipse") {
        return new CEllipse(0, 0, 1, 1);
    }
    if (type == "Triangle") {
        return new CTriangle(QPoint(0,0), QPoint(1,1));
    }
    if (type == "Group") {
        return new Group(0);
    }
    return nullptr;
}
