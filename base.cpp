#include "base.h"

using namespace ENGINE;

Base::Base()
{

}
Base::~Base() {}

void Base::Render() {}


//void Base::Render(){}

bool Base::intersect(int x, int y) {
    return  ( ((x > _Pos.x) && (x < _Pos.x + _Size.w) ) &&
            ((y > _Pos.y) && (y < _Pos.y + _Size.h)) ) ? true : false;
}


void Base::setWidth(int w) {
    _Size.w = w;
    calcDragBar();
}


void Base::setHeight(int h) {
    _Size.h = h;
    calcDragBar();
}

void Base::setSize(int w, int h) {
    _Size.w = w;
    _Size.h = h;
    calcDragBar();
}

void Base::setPos(int x, int y) {
    _Pos.x = x;
    _Pos.y = y;

    calcDragBar();
}

void Base::calcDragBar() {
    // -----------------------------------------------
    // Bereich für Dragging
    // -----------------------------------------------
    interSectHeadline.x  = _Pos.x;
    interSectHeadline.w  = _Size.w;
    interSectHeadline.y  = _Pos.y;
    interSectHeadline.h  = _Size.h;
}

sPoint Base::Pos() {  return _Pos; }
sSize Base:: Size() { return _Size; }

int Base::PosX() {return _Pos.x; }
int Base::PosY() { return _Pos.y; }
int Base::Width(){ return _Size.w; }
int Base::Height() {return _Size.h; }

bool Base::IsEnabled(){
    return _Enable;
}

glm::vec4 Base::color() { return  _Color; }
glm::vec4 Base::backgroundColor(){  return _BackgroundColor;  }
glm::vec4 Base::disableColor() { return _DisableColor; }

void Base::setBackgroundColor(glm::vec4 col) {
    _BackgroundColor = col;
}

void Base::setColor(glm::vec4 col) {
    _Color = col;
}
void Base::setDisablecolor(glm::vec4 disCol) {
    _DisableColor = disCol;
}

void Base::disable(){
    _Enable = false;
}
void Base::enable(){
    _Enable = true;
}

//------------------------------------------------------------------------
//Draging
//------------------------------------------------------------------------
void Base::OnStartDrag(int mx, int my) {

    if ( ! _Dragging) {
        distX = mx - interSectHeadline.x;
        distY = _Pos.y - my;

        _Dragging = true;
    }
}

void Base::OnDrag(int mx, int my) {

    if (_Dragging ) {
        _Pos.x = mx - distX;
        _Pos.y = my + distY;
    }
}

void Base::OnEndDrag(int mx, int my) {
    _Dragging = false;
    setPos(_Pos.x,_Pos.y);
}
bool Base::IsDragging() {
    return  _Dragging;
}
